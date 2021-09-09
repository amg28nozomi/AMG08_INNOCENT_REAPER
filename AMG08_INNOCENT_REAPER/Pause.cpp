#include "Pause.h"
#include "Game.h"
#include "SoundServer.h"
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
		for (auto number = 0; number < 4; ++number) _uis.emplace_back(std::move(std::make_unique<Particle_Image>(_game.GetGame())));
		// _uis.emplace_back(std::move(std::make_unique<Pause_UI>(_game.GetGame())));
		_uis.emplace_back(std::move(std::make_unique<Particle_Image>(_game.GetGame())));
		_uis.emplace_back(std::move(std::make_unique<Particle_Image>(_game.GetGame())));
		// 描画座標修正
		_uis[UI_BG]->SetParameter(image::particle::BG_BLACK, { WINDOW_W / 2, WINDOW_H / 2 });
		_uis[UI_CONTINUE]->SetParameter(image::particle::CONTINUE, {960 ,300});
		_uis[UI_CONTROLS]->SetParameter(image::particle::CONTROLS, {960, 420});
		_uis[UI_QUIT_TO_TITLE]->SetParameter(image::particle::QUIT_TO_TITLE, {960, 540});
		_uis[UI_CURSOL]->SetParameter(image::particle::CURSOR, {960, 300}, 0, 1.2);
		_uis[UI_OPERATION]->SetParameter(image::particle::BG_OPERATION, { WINDOW_W / 2, WINDOW_H / 2 });

		Pause::Init();
	}

	Pause::~Pause() {

	}

	void Pause::Init() {
		_active = INACTIVE;
		for (auto& ui : _uis) ui->Init();
	}

	void Pause::Process() {
		if (_active == INACTIVE) return;	// 非活性状態の場合は処理を行わない
		if (_uis[0]->IsDraw() == false) {
			Init();
			return;
		}
		Input();	// 入力処理
		for (auto& ui : _uis) ui->Process();
	}

	void Pause::Draw() {
		for (auto& ui : _uis) ui->Draw();
	}


	bool Pause::IsActive() {
		PauseOn();
		return _active;
	}

	bool Pause::PauseOn() {
		if (_game.GetTrgKey() != PAD_INPUT_13) return false;
		Sound(system::SOUND_PAUSE);
		if (_active == false) {
			for (auto i = 0; i < _uis.size() - 1; ++i) _uis.at(i)->DrawStart();
			_active = true;
		} else if(_active == true) for (auto i = 0; i < _uis.size(); ++i) _uis.at(i)->End();
		return true;
	}

	bool Pause::Input() {
		// アナログスティックの入力情報を取得
		auto lever = _game.GetLeverUD();
		// 押し倒しがない or 足りない場合は処理を行わない
		if (-50 < lever && lever < 50) return false;
		auto type = false;
		if (lever < -50) type = false;
		else type = true;
		// 番号を切
		_uis.at(UI_CURSOL)->ChangePosition(type);
		return true;
	}

	void Pause::Sound(std::string soundKey) {
		auto sound = se::SoundServer::GetSound(soundKey);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(soundKey));
		return;
	}
}