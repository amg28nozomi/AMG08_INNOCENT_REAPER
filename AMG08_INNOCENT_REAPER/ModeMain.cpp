#include "ModeMain.h"
#include "SoundServer.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "FadeBlack.h"
#include "Scenario.h"
#include "Loads.h"
#include "ModeServer.h"
#include "StageTransition.h"
#include "Pause.h"
#include "EffectServer.h"

#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_uiSoul = std::make_unique<UI>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());
		_eServer = std::make_shared<EffectServer>(_game.GetGame());

		/*Scenario::ObjectData stage1;
		stage1.emplace(objtype::PLAYER, {150, 1900});
		stage1.emplace(objtype::SOLDIER_DOLL, { 500, 0 });
		};*/
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// �e��I�u�W�F�N�g���T�[�o�ɓo�^����
			// �I�u�W�F�N�g�T�[�o�Ƀv���C���[��o�^
			_stageKey = stage::STAGE_3;
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);
			_worldPosition = { 1920 / 2, 1080 / 2 };

			TimeClear();
			_game.GetMapChips()->ChangeMap(_stageKey);
			_game.GetScenario()->AddObjects(_stageKey);
			_eServer->Init();
			_uiSoul->PlayerUpdate();
			_resetFlg = false;
		}
		_bossOpen = false;	// �{�X�̔�
	}

	void ModeMain::Process() {
		IsStageChange();	// �X�e�[�W��؂�ւ��邩�H
		StageReset();	// �X�e�[�W�����������邩�H
		++_modeFrame;

		if (_pause->IsActive() != true) {	// �|�[�Y��ʂ��N�����Ă��Ȃ��Ԃ̂ݎ��s
			_bg->Process();
			_game.GetMapChips()->Process();
			_eServer->Process();
			_game.GetObjectServer()->Process();
			_uiSoul->Process();
			return;
		}
		_pause->Process();	// �|�[�Y��ʍX�V����
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		// �����Ŋe��G�t�F�N�g�̕`�揈�����s��
		_eServer->Draw();
		_game.GetObjectServer()->Draw();
		_uiSoul->Draw();
		if(_pause->Active() == true) _pause->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// �Ó]�������s��
	}


	bool ModeMain::IsStageChange() {
		// �L�[�͐؂�ւ���Ă��邩�H
		if (_changeKey == stage::CHANGE_NULL) return false;
		if (_game.GetModeServer()->PalChange() == true) {
			// �M�~�b�N�̏�Ԃ��X�V����
			BgmManage(_changeKey);	// bgm�؂�ւ�
			_eServer->Init();	// �e��G�t�F�N�g����������
			_game.GetScenario()->ScenarioUpdate(_stageKey);	// ���������ɍX�V��������
			_game.GetMapChips()->ChangeMap(_changeKey);
			_game.GetObjectServer()->ObjectsClear();
			_game.GetObjectServer()->GetPlayer()->SetParameter(_game.GetMapChips()->GetStageTransition()->SetPosition());	// ���@�̍��W���X�V����
			_game.GetScenario()->AddObjects(_changeKey);
			_stageKey = _changeKey;	// �L�[���X�V����
			_changeKey = stage::CHANGE_NULL;
			return true;
		}
		// ���݂̓o�^����Ă���I�u�W�F�N�g�̍폜���s��
		
		// ���@(+�ێ����Ă��鍰)�A
	}

	bool ModeMain::IsKeyNull() {
		bool flag = _changeKey == stage::CHANGE_NULL;
		return flag;
	}

	bool ModeMain::StageChange() {
		// �X�e�[�W�̐؂�ւ�
		return false;
	}

	void ModeMain::SetObjects() {

	}

	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);	// �L�[�擾
		if (_bgmKey == bgm)	return false;	// �L�[���������ꍇ��BGM��؂�ւ����炵������

		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// ���݂̃L�[���~�߂�
		_bgmKey = bgm;	// �Ⴄ�ꍇ��bgm��؂�ւ���
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}

	std::string ModeMain::BgmKey(std::string key) {
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_1) return bgm::SOUND_STAGE_1;
		if (key == stage::STAGE_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_1) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_3) return bgm::SOUND_STAGE_3;
		else return "";	// �o�^����Ă��Ȃ�
	}

	bool ModeMain::GameOver() {
		if (_isReset == true) return false;	// ���łɏ������\�񂪓����Ă���ꍇ�͏������I������
		_isReset = true;	// �t���O���N��
		_game.GetModeServer()->FadeOut();	// �Ó]�������J�n
	}

	bool ModeMain::OpenBossStage() {
		if (_bossOpen == true) return false;
		_bossOpen = true;
	}


	bool ModeMain::StageReset() {
		if (_isReset != true) return false;	// �t���O���s�\��
		if (_game.GetModeServer()->PalChange() != true) return false;	// ���Ԃ𖞂����Ă��Ȃ�
		_eServer->Init();	// �G�t�F�N�g�̏���
		_game.GetObjectServer()->ObjectsClear();	// �I�u�W�F�N�g�̏���
		_game.GetObjectServer()->GetPlayer()->Reset();	// ���@���X�e�[�W�̊J�n�n�_�ɖ߂�
		_game.GetScenario()->AddObjects(_stageKey);		// �I�u�W�F�N�g���Ĕz�u����
		_isReset = false;	// �����I��
	}
}
