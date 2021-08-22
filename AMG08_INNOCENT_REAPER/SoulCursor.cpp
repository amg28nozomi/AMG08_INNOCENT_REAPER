#include "SoulCursor.h"
#include "Game.h"
#include "SoulSkin.h"

namespace {
	constexpr auto S_LEFT = false;
	constexpr auto S_RIGHT = true;

	constexpr auto MOVE_VEC = 50;

	constexpr auto HALF_SOUL = 50;
}

namespace inr {

	SoulCursor::SoulCursor(Game& game) : ObjectBase(game) {
		Init();
	}

	void SoulCursor::Init() {
		_moveVector = { 0, 0 };
		_position = { 960, 540 };
		_divKey = { soul::RED_SOUL, key::SOUND_NUM };
	}

	void SoulCursor::Process() {
		_moveVector = { 0, 0 };

		auto leverLR = _game.GetLeverLR();
		auto key = _game.GetTrgKey();

		AnimationCount();
		Move(leverLR);
		PositionUpdate();
	}

	void SoulCursor::Draw() {
		auto x = _position.IntX();
		auto y = _position.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
	}

	void SoulCursor::Move(int lever) {
		if (lever < -50) _direction = S_LEFT;
		else if (50 < lever) _direction = S_RIGHT;

		if (lever < -50 || 50 < lever) {
			_speed = (lever / MOVE_VEC);
			_moveVector.GetPX() = _speed;
		}
		_speed = 0;
	}

	void SoulCursor::PositionUpdate() {
		_position = _position + _moveVector;

		// 画面内に収まっていない場合は座標を修正する
		if (WINDOW_W < _position.GetX() + HALF_SOUL) _position.GetPX() = WINDOW_W - HALF_SOUL;
		else if (_position.GetX() - HALF_SOUL < 0) _position.GetPX() = HALF_SOUL;
		if (WINDOW_H < _position.GetY() + HALF_SOUL) _position.GetPY() = WINDOW_H - HALF_SOUL;
		else if (_position.GetY() - HALF_SOUL < 0) _position.GetPY() = HALF_SOUL;
	}
}