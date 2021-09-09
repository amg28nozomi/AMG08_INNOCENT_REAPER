#include "ModeMain.h"
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
			_stageKey = stage::STAGE_0;
			_changeKey = stage::CHANGE_NULL;
			_worldPosition = { 1920 / 2, 1080 / 2 };

			TimeClear();
			_game.GetMapChips()->ChangeMap(_stageKey);
			_game.GetScenario()->AddObjects(_stageKey);
			_uiSoul->PlayerUpdate();
			_resetFlg = false;
		}
	}

	void ModeMain::Process() {
		IsStageChange();
		++_modeFrame;

		if (_pause->Active() != true) {
			_bg->Process();
			_game.GetMapChips()->Process();
			_game.GetObjectServer()->Process();
			_uiSoul->Process();
			return;
		}
		_pause->Process();
	}

	void ModeMain::Draw() {
		if (_pause->Active() != true) {
			_bg->Draw();
			_game.GetMapChips()->Draw();
			_game.GetObjectServer()->Draw();
			_uiSoul->Draw();
			return;
		}
		_pause->Draw();
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
			_game.GetScenario()->ScenarioUpdate(_stageKey);	// ���������ɍX�V��������
			_game.GetMapChips()->ChangeMap(_changeKey);
			_game.GetObjectServer()->ObjectsClear();
			_game.GetObjectServer()->GetPlayer()->SetParameter(_game.GetMapChips()->GetStageTransition()->SetPosition());	// ���@�̍��W���X�V����
			_game.GetScenario()->AddObjects(_changeKey);
			_stageKey = _changeKey;	// �L�[���X�V����
			_changeKey = stage::CHANGE_NULL;
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
}
