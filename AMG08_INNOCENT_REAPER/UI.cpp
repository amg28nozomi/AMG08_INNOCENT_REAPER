#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = _game.GetObjectServer()->GetPlayer();
		_pSouls.clear();	// ‹ó‚¾ƒˆ
	}

	void UI::Init() {

	}

	void UI::Process() {

	}

	void UI::Draw() {

	}
}
