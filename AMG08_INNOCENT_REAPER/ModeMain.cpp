#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include <memory>

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_stageKey = "null";
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// 各種オブジェクトをサーバに登録する
			// オブジェクトサーバにプレイヤーを登録
			auto player = std::make_unique<Player>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(player));
			auto enemy = std::make_unique<SoldierDoll>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(enemy));
			TimeClear();
		}
	}

	void ModeMain::Process() {
		++_modeFrame;
		_game.GetMapChips()->Process();
		_game.GetObjectServer()->Process();
	}

	void ModeMain::Draw() {
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
	}
}
