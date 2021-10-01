#include "ModeEnd.h"
#include "Game.h"
#include "ModeServer.h"

namespace inr {

	ModeEnd::ModeEnd(Game& game) : ModeBase(game), _end(false) {
	}

	void ModeEnd::Init() {
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
