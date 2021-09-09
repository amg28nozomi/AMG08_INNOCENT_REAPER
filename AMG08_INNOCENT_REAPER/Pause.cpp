#include "Pause.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto ACTIVE = true;
	constexpr auto INACTIVE = false;
}

namespace inr {

	Pause::Pause(Game& game) : _game(game) {

	}

	Pause::~Pause() {

	}

	void Pause::Init() {

	}

	void Pause::Process() {
		if (IsActive() == INACTIVE) return;	// ”ñŠˆ«ó‘Ô‚Ìê‡‚Íˆ—‚ğs‚í‚È‚¢
	}

	void Pause::Draw() {

	}

	bool Pause::IsActive() {
		if (_active == true) return _active;
		_active = PauseOn();
		return _active;
	}

	bool Pause::PauseOn() {
		auto flag = _game.GetTrgKey() == PAD_INPUT_13;
		return flag;
	}
}