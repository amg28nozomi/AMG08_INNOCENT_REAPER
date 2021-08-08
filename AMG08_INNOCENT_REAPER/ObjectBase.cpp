#include "ObjectBase.h"
#include "Collision.h"
#include "Vector2.h"
#include "Game.h"
#include "MapChips.h"
#include <DxLib.h>
#include <memory>

namespace {
	constexpr auto SPEED_NULL = 0;
}

namespace inr {

	ObjectBase::ObjectBase(Game& game) : _game(game), _mainCollision(Vector2(), Vector2() ) {
		Init();
	}

	ObjectBase::~ObjectBase() {

	}

	void ObjectBase::Init() {
		_type = ObjectType::OBJECT_BASE;
		_speed = SPEED_NULL;
		_gravity = 1;
		_direction = false;
		_stand = false;
		_position = { 0, 0 };
	}

	void ObjectBase::Process() {
		_gravity += FRAME_G;	// 加速度を加算
		// マップチップの上に立っているかどうか
		if (_game.GetMapChips()->IsHit(_mainCollision, _gravity)) {
			// 加速度が0の時だけ立っている
			if (0 < _gravity) {
				_stand = true;
			}
			_gravity = 0;
		} else {
			_stand = false;
		}
	}

	void ObjectBase::Draw() {

	}

	void ObjectBase::ActionBox(double dx, double dy, const std::pair<int, int> wid) {
		Vector2 abmin(dx - wid.first / 2, dy + wid.second / 2);
		Vector2 abmax(dx + wid.first / 2, dy - wid.second / 2);
		// AABB newBox(abmin, abmax);
		auto red = GetColor(255, 255, 0);

		DrawBox(abmin.IntX(), abmin.IntY(), abmax.IntX(), abmax.IntY(), red, FALSE);
	}
}