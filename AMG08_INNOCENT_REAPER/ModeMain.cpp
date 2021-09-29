#include "ModeMain.h"
#include "SoundServer.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "FadeBlack.h"
#include "Scenario.h"
#include "Loads.h"
#include "StageUi.h"
#include "ModeServer.h"
#include "StageTransition.h"
#include "Pause.h"
#include "EffectServer.h"
#include "CrowDoll.h"
#include "ForeGround.h"
#include "ItemServer.h"

#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_bossBattle = false;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_stageUi = std::make_unique<StageUi>(_game.GetGame());
		// _uiSoul = std::make_unique<UI>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());
		_eServer = std::make_shared<EffectServer>(_game.GetGame());
		_messageServer = std::make_unique<ImageServer>(_game.GetGame());
		_fg = std::make_unique<ForeGround>(_game.GetGame());
		_itemServer = std::make_unique<ItemServer>();
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// �e��I�u�W�F�N�g���T�[�o�ɓo�^����
			// �I�u�W�F�N�g�T�[�o�Ƀv���C���[��o�^
			_stageKey = stage::STAGE_2_2;
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);
			_worldPosition = { 1920 / 2, 1080 / 2 };

			TimeClear();
			_fg->SetKey(_stageKey);
			_game.GetMapChips()->ChangeMap(_stageKey);
			_game.GetScenario()->AddObjects(_stageKey);
			_eServer->Init();
			// _uiSoul->PlayerUpdate();
			_stageUi->ChangeNumber(_stageKey);
			//_itemImages->AddImage()
			_bossOpen = false;	// �{�X�̔�
			_bossBattle = false;
			_resetFlg = false;
		} else {
			_game.GetScenario()->Init();
			_eServer->Init();	// �e��G�t�F�N�g����������
			_game.GetObjectServer()->AllClear();
			_game.GetGimmickServer()->Clear();
			_pause->Init();
			_messageServer->Init();
			_bg->ChangeGraph();
			// _uiSoul->Reset();
			_resetFlg = true;
		}
	}

	void ModeMain::Process() {
		IsStageChange();	// �X�e�[�W��؂�ւ��邩�H
		StageReset();	// �X�e�[�W�����������邩�H
		if (_stageUi->FadeDraw() != true && _game.GetModeServer()->IsFadeEnd() == true) _stageUi->DrawStart();
		++_modeFrame;

		if (_messageServer->IsActive() == true) {
			_messageServer->Process();
			return;
		} else if (_pause->IsActive() == true) {
			_pause->Process();	// �|�[�Y��ʍX�V����
			return;
		}
		// �ǂ̃t���O���I���ɂȂ��Ă��Ȃ��ꍇ�̂ݏ��������s����
		_bg->Process();
		_game.GetGimmickServer()->Process();
		_game.GetMapChips()->Process();
		_eServer->Process();
		_game.GetObjectServer()->Process();
		_itemServer->Process();
		_fg->Process();
		// _uiSoul->Process();
		_stageUi->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();	// �w�i
		_game.GetGimmickServer()->Draw();	// �M�~�b�N
		_game.GetMapChips()->Draw();	// �}�b�v�`�b�v
		_eServer->DrawBack();	// �G�t�F�N�g(��)
		_game.GetObjectServer()->Draw();	// �I�u�W�F�N�g
		_itemServer->Draw();
		_eServer->DrawFormer();	// �G�t�F�N�g(�O)
		_fg->Draw();
		// _uiSoul->Draw();	// HP(UI)
		_stageUi->Draw();	// �X�e�[�WUI
		if(_pause->Active() == true) _pause->Draw();
		if (_messageServer->IsActive() == true) _messageServer->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// �Ó]�������s��
	}


	bool ModeMain::IsStageChange() {
		// �L�[�͐؂�ւ���Ă��邩�H
		if (_changeKey == stage::CHANGE_NULL) return false;
		if (CheckSoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1)) == 0) PlaySoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1), DX_PLAYTYPE_BACK);
		if (_game.GetModeServer()->PalChange() == true) {
			// �M�~�b�N�̏�Ԃ��X�V����
			BgmManage(_changeKey);	// bgm�؂�ւ�
			_stageUi->ChangeNumber(_changeKey);
			_fg->SetKey(_changeKey);
			_eServer->Init();	// �e��G�t�F�N�g����������
			_game.GetScenario()->ScenarioUpdate(_stageKey);	// ���������ɍX�V��������
			_game.GetMapChips()->ChangeMap(_changeKey);
			_game.GetGimmickServer()->Clear();
			_itemServer->ItemClear();
			_game.GetObjectServer()->ObjectsClear();
			_game.GetObjectServer()->GetPlayer()->SetParameter(_game.GetMapChips()->GetStageTransition()->SetPosition(), _changeKey);	// ���@�̍��W���X�V����
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

	bool ModeMain::BossBattle() {
		if (_stageKey != stage::STAGE_3) return false;
		_bossBattle = true;
		_bg->ScrollOff();

		auto sound = se::SoundServer::GetSound(_bgmKey);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
	}

	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);	// �L�[�擾
		if (_bgmKey == bgm)	return false;	// �L�[���������ꍇ��BGM��؂�ւ����炵������
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// ���݂̃L�[���~�߂�
		_bgmKey = bgm;	// �Ⴄ�ꍇ��bgm��؂�ւ���
		if (bgm == bgm::SOUND_STAGE_3) return true;
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}

	std::string ModeMain::BgmKey(std::string key) {
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_T) return bgm::SOUND_STAGE_0;
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
		_bossBattle = false;
		if (CheckSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3)) == 1) StopSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3));
		// _uiSoul->Init();
		_bg->ScrollOn();	// �X�N���[���ĊJ
		_eServer->Init();	// �G�t�F�N�g�̏���
		_game.GetGimmickServer()->Clear();
		_game.GetObjectServer()->ObjectsClear();	// �I�u�W�F�N�g�̏���
		_game.GetObjectServer()->GetPlayer()->Reset();	// ���@���X�e�[�W�̊J�n�n�_�ɖ߂�
		_game.GetScenario()->AddObjects(_stageKey);		// �I�u�W�F�N�g���Ĕz�u����
		_isReset = false;	// �����I��
	}
}
