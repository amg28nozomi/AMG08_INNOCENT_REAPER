#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "ObjectServer.h"
#include "Logo.h"
#include <memory>
#include <DxLib.h>

namespace inr {

	ModeTitle::ModeTitle(Game& game) : ModeBase(game) {
		_titleGh = std::make_unique<Logo>(_game.GetGame());
		_ui = std::make_unique<TitleLogo>(_game.GetGame());
	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {

	}

	void ModeTitle::Process() {
		_titleGh->Process();
		_ui->Process();
		_game.GetObjectServer()->Process();
		//// A�{�^���͉����ꂽ��
		//if (_game.GetTrgKey() & PAD_INPUT_3) {
		//	// A�{�^���������ꂽ���A���[�h��؂�ւ�
		//	_game.GetModeServer()->ModeChange(mode::MAIN);
		//}
	}

	void ModeTitle::Draw() {
		_titleGh->Draw();
		_ui->Draw();
		_game.GetObjectServer()->Draw();
	}
}
