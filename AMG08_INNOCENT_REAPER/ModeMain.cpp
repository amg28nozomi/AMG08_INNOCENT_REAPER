#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "Scenario.h"
#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_stageKey = "null";

		/*Scenario::ObjectData stage1;
		stage1.emplace(objtype::PLAYER, {150, 1900});
		stage1.emplace(objtype::SOLDIER_DOLL, { 500, 0 });
		};*/
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

			auto enemy1 = std::make_unique<SoldierDoll>(_game.GetGame());
			enemy1->SetStatus({ 5000, 1700 }, "empty");
			_game.GetObjectServer()->Add(std::move(enemy1));

			auto soul_r = std::make_unique<SoulSkin>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(soul_r));
			_bg = std::make_unique<BackGround>(_game.GetGame());
			TimeClear();
		}

		_worldPosition = { 1920 / 2, 1080 / 2 };
	}

	void ModeMain::Process() {
		++_modeFrame;
		// ワールド座標更新
		// _worldPosition = _game.GetObjectServer()->GetPlayer().GetPosition();

		_bg->Process();
		_game.GetMapChips()->Process();
		_game.GetObjectServer()->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
	}
}
