#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "ObjectServer.h"
#include "Logo.h"
#include "FadeBlack.h"
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
			_game.GetObjectServer()->Process();
			_game.GetFadeBlack()->Process();

		/*_titleGh->Process();
		_ui->Process();
		_game.GetObjectServer()->Process();*/
	}

	void ModeTitle::Draw() {
		_titleGh->Draw();
		_game.GetObjectServer()->Draw();
		_game.GetFadeBlack()->Process();
	}
}
