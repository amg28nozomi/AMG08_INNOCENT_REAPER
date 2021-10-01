#include "ModeEnd.h"
#include "Game.h"
#include "ModeServer.h"
#include "MoveImage.h"

namespace inr {

	ModeEnd::ModeEnd(Game& game) : ModeBase(game), _end(false) {
		_stuffRoll = std::make_unique<MoveImage>(_game.GetGame());
		_stuffRoll->SetParameter(end::STUFF_ROLL, { HALF_WINDOW_W, (-end::STUFF_ROLL_HEIGHT + HALF_WINDOW_H) });
		_bgmKey = 
	}

	void ModeEnd::Init() {
		_end = true;
		BgmManage();	// BGMの切り替え
	}

	void ModeEnd::Process() {
		if (IsEnd() == true) return;
		// 移動処理を行う
	}

	void ModeEnd::Draw() {

	}

	bool ModeEnd::IsEnd() {
		if (_end != true) return false;
		_game.GetModeServer()->ModeChange(mode::TITLE);	// タイトルに遷移する
		return true;
	}
}
