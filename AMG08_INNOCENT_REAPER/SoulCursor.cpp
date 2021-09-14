#include "SoulCursor.h"
#include "Game.h"
#include "SoulSkin.h"
#include <DxLib.h>

namespace {
	/*constexpr auto S_LEFT = false;
	constexpr auto S_RIGHT = true;*/

	constexpr auto MOVE_VEC = 100;

	constexpr auto HALF_SOUL = 25;

	constexpr auto SPEED_MAX = 10;

	constexpr auto SMF_FLOAT = 120;
}

namespace inr {

	SoulCursor::SoulCursor(Game& game) : ObjectBase(game) {
		Init();

		_motionKey = {{ui::CURSOR, {SMF_FLOAT, 0}}};
		_type = ObjectType::SOUL;
	}

	void SoulCursor::Init() {
		_moveVector = { 0, 0 };
		_position = { 960, 540 };
		_mainCollision = { _position, HALF_SOUL, HALF_SOUL, true };
		_divKey = { ui::CURSOR, key::SOUND_NUM };
		_setBlend = false;
		_input = false;
		_rgb.Min();
	}

	void SoulCursor::Process() {
		_moveVector = { 0, 0 };

		auto leverLR = _game.GetLeverLR();
		auto leverUD = _game.GetLeverUD();
		auto key = _game.GetTrgKey();

		AnimationCount();
		Move(leverLR, leverUD);
		PositionUpdate();
		if (!_rgb.IsBrendMax()) _rgb.Update(rgb::ADD, 4);
		else if (!_input) _input = true;
	}

	void SoulCursor::Draw() {
		auto x = _position.IntX();
		auto y = _position.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得

		ChangeBlendGraph();
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
		ChangeBlendGraph();

#ifdef _DEBUG
		/*auto c = DxLib::GetColor(255, 255, 0);
		DrawDebugBox(_mainCollision, c);*/
#endif
	}

	void SoulCursor::Move(int lever1, int lever2) {
		if (_input) {
			if (lever1 < -50 || 50 < lever1) {
				_moveVector.GetPX() = (lever1 / MOVE_VEC);
			}
			if (lever2 < -50 || 50 < lever2) {
				_moveVector.GetPY() = (lever2 / MOVE_VEC);
			}
		}

	}

	void SoulCursor::PositionUpdate() {
		_position = _position + _moveVector;

		// 画面内に収まっていない場合は座標を修正する
		if (WINDOW_W < _position.GetX() + HALF_SOUL) _position.GetPX() = WINDOW_W - HALF_SOUL;
		else if (_position.GetX() - HALF_SOUL < 0) _position.GetPX() = HALF_SOUL;
		if (WINDOW_H < _position.GetY() + HALF_SOUL) _position.GetPY() = WINDOW_H - HALF_SOUL;
		else if (_position.GetY() - HALF_SOUL < 0) _position.GetPY() = HALF_SOUL;

		_mainCollision.Update(_position, _direction);
	}

	void SoulCursor::ChangeBlendGraph() {
		int r, g, b;
		switch (_setBlend) {
		case true:
			r = rgb::MAX_BLEND;
			g = rgb::MAX_BLEND;
			b = rgb::MAX_BLEND;
			_setBlend = false;
			break;
		case false:
			// 現在の色彩を代入
			r = _rgb.Red();
			g = _rgb.Green();
			b = _rgb.Blue();
			_setBlend = true;	// フラグオン
			break;
		}
		SetDrawBright(r, g, b);
	}
}