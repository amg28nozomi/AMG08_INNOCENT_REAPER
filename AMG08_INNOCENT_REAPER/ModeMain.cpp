#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "Scenario.h"
#include "Loads.h"
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
			auto scenarioD = Loads::LoadScenario();
			_game.GetScenario()->LoadObjectData(stage::STAGE_1, scenarioD);
			_game.GetScenario()->AddObjects(stage::STAGE_1);

			// auto soul_r = std::make_shared<SoulSkin>(_game.GetGame());
			// _game.GetObjectServer()->Add(std::move(soul_r));
			_bg = std::make_unique<BackGround>(_game.GetGame());
			_uiSoul = std::make_unique<UI>(_game.GetGame());
			TimeClear();
			_resetFlg = false;
		}
		_stageKey = stage::STAGE_1;
		_worldPosition = { 1920 / 2, 1080 / 2 };
	}

	void ModeMain::Process() {
		++_modeFrame;
		// ワールド座標更新
		// _worldPosition = _game.GetObjectServer()->GetPlayer().GetPosition();

		_bg->Process();
		_game.GetMapChips()->Process();
		_game.GetObjectServer()->Process();
		_uiSoul->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
		_uiSoul->Draw();
	}
}
