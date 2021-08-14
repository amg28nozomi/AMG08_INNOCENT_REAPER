#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include "MapChips.h"
#include <DxLib.h>

namespace {
	constexpr auto BACKGROUND_SPEED = 0.5;
	constexpr auto MAX_SCROLL = 1920 + 960;
	constexpr auto MIN_SCROLL = -960;
}

namespace inr {

	BackGround::BackGround(Game& game) : Image(game){
		Init();
	}

	void BackGround::Init() {
		// 座標の初期化
		_pos = { HALF_WINDOW_W, 0 };
		_pos2 = { MAX_SCROLL , 0 };

		// 
		_graphKey = BACK_GROUND;
	}

	void BackGround::Process() {
		// ワールド座標の移動量を取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto worldY = _game.GetMapChips()->BeforeWorldPos().IntY();

		// ワールド座標はスクロール開始地点を超えているか？
		if (_game.GetMapChips()->IsScroll() == true) {
			// 移動量分だけ座標をずらす
			_pos.GetPX() += moveX * BACKGROUND_SPEED;
			_pos2.GetPX() += moveX * BACKGROUND_SPEED;
			// 一枚目の修正処理
			if (_pos.IntX() < -HALF_WINDOW_W) {
				// 漏れた値を算出
				auto vec = -HALF_WINDOW_W - _pos.GetX();
				_pos.GetPX() = MAX_SCROLL - vec;
			}
			else if (MAX_SCROLL < _pos.IntX()) { 
				auto vec = _pos.GetX() - MAX_SCROLL;
				_pos.GetPX() = -HALF_WINDOW_W + vec; 
			}
			// 二枚目の修正処理
			if (_pos2.IntX() < -HALF_WINDOW_W) { 
				auto vec = -HALF_WINDOW_W - _pos2.GetX();
				_pos2.GetPX() = MAX_SCROLL - vec; 
			}
			else if (WINDOW_W + HALF_WINDOW_W < _pos2.IntX()) { 
				auto vec = _pos2.GetX() - MAX_SCROLL;
				_pos2.GetPX() = -HALF_WINDOW_W + vec; 
			}
		}
	}

	void BackGround::Draw() {
		auto x1 = _pos.IntX();
		auto y1 = _pos.IntY();

		auto x2 = _pos2.IntX();
		auto y2 = _pos2.IntY();

		auto gh = graph::ResourceServer::GetHandles(_graphKey, 0);
		DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
		DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
	}
}
