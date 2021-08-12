#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include <memory>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

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
			_bg = std::make_unique<BackGround>();
			TimeClear();
		}

		_worldPosition = { 1920 / 2, 1080 / 2 };
	}

	void ModeMain::Process() {
		++_modeFrame;
		// ワールド座標更新
		_worldPosition = _game.GetObjectServer()->GetPlayer().GetPosition();

		_game.GetMapChips()->Process();
		_game.GetObjectServer()->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
	}

	bool ModeMain::Clamp(Vector2& pos) {
		auto x = pos.GetX() - _worldPosition.GetX();
		auto y = pos.GetY() - _worldPosition.GetY();
		// 対象の座標からカメラ座標を引いた値はプラスかどうか？
		if (x < 0 && 0 < y) {
			// スクリーン座標内に存在しない
			return false;
		}
		// 対象は現在のスクリーン座標上に存在しているか？
		if( WINDOW_MIN <= x <= WINDOW_W && WINDOW_MIN <= y <= WINDOW_H) {
			// ワールド座標からスクリーン座標に修正
			pos.GetPX() = x;
			pos.GetPY() = y;
			return true;
		}
		return false;
	}
}
