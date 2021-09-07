#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "FadeBlack.h"
#include "Scenario.h"
#include "Loads.h"
#include "ModeServer.h"

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
			auto scenarioD = Loads::LoadScenario1();	// 対応するシナリオを読み込み
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
		IsStageChange();
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


	bool ModeMain::IsStageChange() {
		// キーは切り替わっているか？
		if (_changeKey == stage::CHANGE_NULL) return false;
		_game.GetModeServer()->FadeOut();	// 暗転処理を行う
		// 現在の登録されているオブジェクトの削除を行う
		
		// 自機(+保持している魂)、
	}

	void ModeMain::SetObjects() {

	}
}
