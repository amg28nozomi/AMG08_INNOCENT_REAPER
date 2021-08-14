#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "TitleLogo.h"
#include <memory>
#include <DxLib.h>

namespace inr {

	ModeTitle::ModeTitle(Game& game) : ModeBase(game) {
		_titleLG = std::make_unique<TitleLogo>(_game.GetGame());
	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {

	}

	void ModeTitle::Process() {
		// A�{�^���͉����ꂽ��
		if (_game.GetTrgKey() & PAD_INPUT_3) {
			// A�{�^���������ꂽ���A���[�h��؂�ւ�
			_game.GetModeServer()->ModeChange(mode::MAIN);
		}
	}

	void ModeTitle::Draw() {
		_titleLG->Draw();
	}
}
