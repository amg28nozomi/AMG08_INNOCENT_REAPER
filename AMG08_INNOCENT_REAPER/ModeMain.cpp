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
#include "TutorialServer.h"
#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
	constexpr auto END_STAY = 300;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_bossBattle = false;
		_bossOpen = false;
		_isEnding = false;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_stageUi = std::make_unique<StageUi>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());
		_eServer = std::make_shared<EffectServer>(_game.GetGame());
		_messageServer = std::make_unique<ImageServer>(_game.GetGame());
		_fg = std::make_unique<ForeGround>(_game.GetGame());
		_itemServer = std::make_unique<ItemServer>();
		_tutorialServer = std::make_unique<TutorialServer>();
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// ��������̃X�e�[�W��ݒ�
#ifdef _DEBUG
			_stageKey = stage::STAGE_2;
#endif
#ifndef _DEBUG
			_stageKey = stage::STAGE_0;
#endif
			// �X�e�[�W�؂�ւ��p�̃L�[��������
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);	// �X�e�[�W�ɑΉ�����BGM��炷

			TimeClear();
			_bg->ScrollOn();													// �w�i�̃X�N���[���ĊJ
			_fg->SetKey(_stageKey);											// �O�i�̐؂�ւ�
			_game.GetMapChips()->ChangeMap(_stageKey);	// �}�b�v�`�b�v�̐؂�ւ�
			_game.GetScenario()->AddObjects(_stageKey);	// �e��I�u�W�F�N�g�̐���
			_eServer->Init();														// �G�t�F�N�g������
			_stageUi->ChangeNumber(_stageKey);					// �X�e�[�WUI�̉摜�؂�ւ�
			// �e��t���O�̐ݒ�
			_bossOpen = false;	
			_bossBattle = false;
			_resetFlg = false;
			_isEnding = false;
			_endCount = 0;
		} else {
			_game.GetScenario()->Init();
			_eServer->Init();	// �e��G�t�F�N�g����������
			_game.GetObjectServer()->AllClear();
			_game.GetGimmickServer()->Clear();
			_pause->Init();
			_messageServer->Init();
			_itemServer->ItemClear();	// �S����
			_bg->ChangeGraph();
			_tutorialServer->Clear();
			_resetFlg = true;

			auto nowbgm = se::SoundServer::GetSound(_bgmKey);
			StopSoundMem(nowbgm);
		}
	}

	void ModeMain::Process() {
		IsStageChange();	// �X�e�[�W��؂�ւ��邩�H
		StageReset();			// �X�e�[�W�����������邩�H
		if (_isEnding == true) IsEnding();	// �G���f�B���O�ɓ˓����邩�H
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
		_tutorialServer->Process();
		_game.GetMapChips()->Process();
		_eServer->Process();
		_game.GetObjectServer()->Process();
		_itemServer->Process();
		_fg->Process();
		_stageUi->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();											// �w�i
		_game.GetGimmickServer()->Draw();	// �M�~�b�N
		_game.GetMapChips()->Draw();			// �}�b�v�`�b�v
		_tutorialServer->Draw();					// �`���[�g���A��UI
		_eServer->DrawBack();							// �G�t�F�N�g(��)
		_game.GetObjectServer()->Draw();	// �I�u�W�F�N�g
		_itemServer->Draw();							// �A�C�e��
		_eServer->DrawFormer();						// �G�t�F�N�g(�O)
		_fg->Draw();											// �O�i
		_stageUi->Draw();									// �X�e�[�WUI
		// �|�[�Y���
		if(_pause->Active() == true) _pause->Draw();
		// �A�C�e���e�L�X�g
		if (_messageServer->IsActive() == true) _messageServer->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// �Ó]�������s��
		_game.GetObjectServer()->GetPlayer()->InputOff();
	}


	bool ModeMain::IsStageChange() {
		// �L�[�͐؂�ւ���Ă��邩�H
		if (_changeKey == stage::CHANGE_NULL) return false;
		// �X�e�[�W�J��SE��炷
		if (CheckSoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1)) == 0) PlaySoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1), DX_PLAYTYPE_BACK);
		// ��ʂ����S�ɈÓ]������e�평�������s��
		if (_game.GetModeServer()->PalChange() == true) {
			// �M�~�b�N�̏�Ԃ��X�V����
			BgmManage(_changeKey);	// bgm�؂�ւ�
			_stageUi->ChangeNumber(_changeKey);
			_fg->SetKey(_changeKey);
			_eServer->Init();	// �e��G�t�F�N�g����������
			_tutorialServer->Clear();
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
	}
	// �L�[���󂩂̔���
	bool ModeMain::IsKeyNull() {
		// �L�[�͐؂�ւ���Ă��邩
		bool flag = _changeKey == stage::CHANGE_NULL;
		return flag;
	}
	// �{�X��J�n�t���O
	bool ModeMain::BossBattle() {
		// �{�X�X�e�[�W�ł͂Ȃ��ꍇ�͏I��
		if (_stageKey != stage::STAGE_3) return false;
		// �G���f�B���O������ꍇ�������𒆒f
		if (_isEnding == true) return false;
		_bossBattle = true;								// �{�X��J�n
		_bg->ScrollOff();									// �X�N���[�����~
		_game.GetScenario()->BossBlock();	// �u���b�N�𐶐����ē��𕕍�����

		auto sound = se::SoundServer::GetSound(_bgmKey);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
	}
	// �{�X����I�����邩
	bool ModeMain::BossEnd() {
		// ���݋���̂̓{�X�X�e�[�W���H
		if (_stageKey != stage::STAGE_3) return false;
		// �{�X�풆��?
		if (_bossBattle != true) return false;
		_bossBattle = false;		// �{�X����I������
		_isEnding = true;				// �G���h�t���O�𗧂Ă�
		_endCount = END_STAY;		// �G���f�B���O�ɑJ�ڂ���܂ł̎c��t���[����
		// �{�X��BGM���~����
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));
		return true;	// �{�X��I��
	}
	// BGM�̐؂�ւ�
	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);
		if (_bgmKey == bgm)	return false;									// �L�[���������ꍇ��BGM��؂�ւ����炵������
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// ���݂̃L�[���~�߂�
		// �Ⴄ�ꍇ��bgm��؂�ւ���
		_bgmKey = bgm;
		if (bgm == bgm::SOUND_STAGE_3) return true;
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}
	// BGM�L�[�̐؂�ւ�
	std::string ModeMain::BgmKey(std::string key) {
		// �L�[�ƑΉ�����T�E���h�L�[��Ԃ�
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_T) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_1) return bgm::SOUND_STAGE_1;
		if (key == stage::STAGE_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_1) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_3) return bgm::SOUND_STAGE_3;
		else return "";	// �o�^����Ă��Ȃ�
	}
	// �Q�[���I�[�o�[
	bool ModeMain::GameOver() {
		if (_isReset == true) return false;	// ���łɏ������\�񂪓����Ă���ꍇ�͏������I������
		_isReset = true;										// �t���O���N��
		_game.GetModeServer()->FadeOut();		// �Ó]�������J�n
		return true;
	}
	// �{�X�h�A�t���O�̊J��
	bool ModeMain::OpenBossStage() {
		// ���ɊJ���Ă���ꍇ�͏I��
		if (_bossOpen == true) return false;
		_bossOpen = true;
		return true;		// �J������
	}
	// �G���f�B���O�ɑJ�ڂ��邩
	bool ModeMain::IsEnding() {
		if (_endCount == 0) return true;
		--_endCount;
		if (_endCount == 0) {
			// �J�E���^��0�ɂȂ����ꍇ��120�t���[����ɑJ�ڏ����J�n
			_game.GetModeServer()->ModeChange(mode::FIN, 120);
			return true;
		}
		return false;
	}
	// �X�e�[�W�̃��Z�b�g
	bool ModeMain::StageReset() {
		if (_isReset != true) return false;														// �t���O���s�\��
		if (_game.GetModeServer()->PalChange() != true) return false;	// ���Ԃ𖞂����Ă��Ȃ�
		_bossBattle = false;														//�@�{�X����I������
		// �{�X��BGM�����Ă���ꍇ�͒�~����
		if (CheckSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3)) == 1) StopSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3));
		// _uiSoul->Init();
		_bg->ScrollOn();																// �X�N���[���ĊJ
		_eServer->Init();																// �G�t�F�N�g�̏���
		_tutorialServer->Clear();												// �`���[�g���A��UI�̏���
		_itemServer->ItemClear();												// �A�C�e��������
		_game.GetGimmickServer()->Clear();							// �M�~�b�N�̏���
		_game.GetObjectServer()->ObjectsClear();				// �I�u�W�F�N�g�̏���
		_game.GetObjectServer()->GetPlayer()->Reset();	// ���@���X�e�[�W�̊J�n�n�_�ɖ߂�
		_game.GetScenario()->AddObjects(_stageKey);			// �I�u�W�F�N�g���Ĕz�u����
		_isReset = false;																// �����I��
		return true;
	}
}
