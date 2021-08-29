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


	constexpr auto BACK_00 = 0.30;
	constexpr auto BACK_01 = 0.4;
	constexpr auto BACK_02 = 0.6;
}

namespace inr {

	BackGround::BackGround(Game& game) : Image(game){
		Init();
	}

	void BackGround::Init() {
		// 座標の初期化
		for (int n = 0; n < 3; ++n) {
			_positions.first.emplace_back(HALF_WINDOW_W, 0);
			_positions.second.emplace_back(MAX_SCROLL, 0);
		}
		// 
		_graphKey = background::BACK_GROUND_1;

		_scrSpeed = { BACK_00, BACK_01, BACK_02 };
	}

	void BackGround::Process() {
		// ワールド座標の移動量を取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto moveY = _game.GetMapChips()->BeforeWorldPos().IntY() * -1;


		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ワールドX座標はスクロール開始地点を超えているか？
			if (_game.GetMapChips()->IsScrollX() == true) {
				// 移動量分だけ座標をずらす
				_positions.first[i].GetPX() += moveX * _scrSpeed[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed[i];
				// 一枚目の修正処理
				if (_positions.first[i].IntX() < -HALF_WINDOW_W) {
					// 漏れた値を算出
					auto vec = -HALF_WINDOW_W - _positions.first[i].GetX();
					_positions.first[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// 二枚目の修正処理
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}

			if (_game.GetMapChips()->IsScrollY() == true) {
				_positions.first[i].GetPY() += moveY * _scrSpeed[i];
				_positions.second[i].GetPY() += moveY * _scrSpeed[i];

				if (_positions.first[i].IntY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				else if (WINDOW_H < _positions.first[i].IntY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = WINDOW_H;
				}
			}
		}
	}

	void BackGround::Draw() {
		for (auto number = 0; number < static_cast<int>(_positions.first.size()); ++number) {
			auto x1 = _positions.first[number].IntX();
			auto y1 = _positions.first[number].IntY();

			auto x2 = _positions.second[number].IntX();
			auto y2 = _positions.second[number].IntY();
			auto gh = graph::ResourceServer::GetHandles(_graphKey, number);
			DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
			DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
		}
		DrawFormatString(500, 500, GetColor(255, 0, 255), "backGround_y : %d\n", _positions.second[0].IntY());
	}
}
