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

	BackGround::BackGround(Game& game) : _game(game){
		_position1 = { 1920 / 2, 0 };
		_position2 = { MAX_SCROLL , 0 };
	}

	void BackGround::Process() {
		// ワールド座標の移動量を取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto worldY = _game.GetMapChips()->BeforeWorldPos().IntY();

		// ワールド座標はスクロール開始地点を超えているか？
		if (_game.GetMapChips()->IsScroll() == true) {
			// 移動量分だけ座標をずらす
			_position1.GetPX() += moveX * BACKGROUND_SPEED;
			_position2.GetPX() += moveX * BACKGROUND_SPEED;
			// 一枚目の修正処理
			if (_position1.IntX() < -HALF_WINDOW_W) {
				// 漏れた値を算出
				auto vec = -HALF_WINDOW_W - _position1.GetX();
				_position1.GetPX() = MAX_SCROLL - vec;
			}
			else if (MAX_SCROLL < _position1.IntX()) { 
				auto vec = _position1.GetX() - MAX_SCROLL;
				_position1.GetPX() = -HALF_WINDOW_W + vec; 
			}
			// 二枚目の修正処理
			if (_position2.IntX() < -HALF_WINDOW_W) { 
				auto vec = -HALF_WINDOW_W - _position2.GetX();
				_position2.GetPX() = MAX_SCROLL - vec; 
			}
			else if (WINDOW_W + HALF_WINDOW_W < _position2.IntX()) { 
				auto vec = _position2.GetX() - MAX_SCROLL;
				_position1.GetPX() = -HALF_WINDOW_W + vec; 
			}
		}
	}

	void BackGround::Draw() {
		auto x1 = _position1.IntX();
		auto y1 = _position1.IntY();

		auto x2 = _position2.IntX();
		auto y2 = _position2.IntY();

		auto gh = graph::ResourceServer::GetHandles(BACK_GROUND, 0);
		DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
		DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
	}
}
