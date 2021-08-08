#include "ModeTitle.h"
#include "Game.h"
#include <DxLib.h>

namespace inr {

	ModeTitle::ModeTitle(Game& game) : ModeBase(game) {

	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {

	}

	void ModeTitle::Process() {
		if (_game.GetTrgKey() & PAD_INPUT_3) {
			// Aボタンが押された時、モードを切り替え
			_game.GetModeServer();
			
		}
	}

	void ModeTitle::Draw() {

	}
}
