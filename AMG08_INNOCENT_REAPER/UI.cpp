#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = _game.GetObjectServer()->GetPlayer();
		_pSouls.clear();
	}

	void UI::Init() {
		// Šeíˆ—‚Ì‰Šú‰»
	}

	void UI::Process() {
		// 
		
	}

	void UI::Draw() {

	}
}
