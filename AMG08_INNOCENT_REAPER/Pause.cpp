#include "Pause.h"
#include "Game.h"
#include "Particle_Image.h"
#include "Pause_UI.h"
#include <DxLib.h>

namespace {
	constexpr auto ACTIVE = true;
	constexpr auto INACTIVE = false;
}

namespace inr {

	Pause::Pause(Game& game) : _game(game) {
		_bg = std::make_unique<Particle_Image>(_game.GetGame());
		_operation = std::make_unique<Particle_Image>(_game.GetGame());
		for (auto number = 0; number < 4; ++number) _uis.emplace_back(std::move(std::make_unique<Pause_UI>()));
		Init();
	}

	Pause::~Pause() {

	}

	void Pause::Init() {
		_active = INACTIVE;
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