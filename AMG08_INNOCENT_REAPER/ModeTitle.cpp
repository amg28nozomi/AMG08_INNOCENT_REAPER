#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "TitleLogo.h"
#include "Logo.h"
#include <memory>
#include <DxLib.h>

namespace inr {

	ModeTitle::ModeTitle(Game& game) : ModeBase(game) {
		_titleGh = std::make_unique<Logo>(_game.GetGame());
	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {

	}

	void ModeTitle::Process() {
		_titleGh->Process();
		//// Aボタンは押されたか
		//if (_game.GetTrgKey() & PAD_INPUT_3) {
		//	// Aボタンが押された時、モードを切り替え
		//	_game.GetModeServer()->ModeChange(mode::MAIN);
		//}
	}

	void ModeTitle::Draw() {
		_titleGh->Draw();
	}
}
