#include "Pause.h"
#include "Game.h"
#include "Particle_Image.h"
#include "Pause_UI.h"
#include <DxLib.h>

namespace {
	constexpr auto ACTIVE = true;
	constexpr auto INACTIVE = false;

	constexpr auto UI_BG = 0;
	constexpr auto UI_CONTINUE = 1;
	constexpr auto UI_CONTROLS = 2;
	constexpr auto UI_QUIT_TO_TITLE = 3;
	constexpr auto UI_CURSOL = 4;
	constexpr auto UI_OPERATION = 5;

}

namespace inr {

	Pause::Pause(Game& game) : _game(game) {
		for (auto number = 0; number < 2; ++number) _uis.emplace_back(std::move(std::make_unique<Particle_Image>(_game.GetGame())));
		for (auto number = 0; number < 4; ++number) _uis.emplace_back(std::move(std::make_unique<Pause_UI>(_game.GetGame())));
		// •`‰æÀ•WC³
		_uis[UI_BG]->SetParameter(image::particle::BG_BLACK, { WINDOW_W / 2, WINDOW_H / 2 });
		_uis[UI_CONTINUE]->SetParameter(image::particle::CONTINUE, {960 ,300});
		_uis[UI_CONTROLS]->SetParameter(image::particle::CONTROLS, {960, 420});
		_uis[UI_QUIT_TO_TITLE]->SetParameter(image::particle::QUIT_TO_TITLE, {960, 540});
		_uis[UI_CURSOL]->SetParameter(image::particle::CURSOR, {960, 300}, 0, 1.2);
		_uis[UI_OPERATION]->SetParameter(image::particle::BG_OPERATION, { WINDOW_W / 2, WINDOW_H / 2 }, 0, 1.0);

		Init();
	}

	Pause::~Pause() {

	}

	void Pause::Init() {
		_active = INACTIVE;
		for (auto&& ui : _uis) ui->Init();
	}

	void Pause::Process() {
		if (IsActive() == INACTIVE) return;	// ”ñŠˆ«ó‘Ô‚Ìê‡‚Íˆ—‚ğs‚í‚È‚¢
		for (auto&& ui : _uis) ui->Process();
	}

	void Pause::Draw() {
		for (auto&& ui : _uis) ui->Draw();
	}

	bool Pause::IsActive() {
		if (IsActive() == INACTIVE) return;	// ”ñŠˆ«ó‘Ô‚Ìê‡‚Íˆ—‚ğs‚í‚È‚¢
		if (_active == true) return _active;
		_active = PauseOn();
		return _active;
	}

	bool Pause::PauseOn() {
		auto flag = _game.GetTrgKey() == PAD_INPUT_13;
		return flag;
	}
}