#include "BigDoll.h"

namespace {
	constexpr auto BIG_EMPTY_WIDTH = 200;
	constexpr auto BIG_EMPTY_HEIGHT = 100;
}

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {

	}

	BigDoll::~BigDoll() {

	}

	void BigDoll::Init() {
		// “–‚½‚è”»’è
		_mainCollision = { _position, enemy::BIG_WIDTH / 2, enemy::BIG_HEIGHT / 2, true };
		_collisions = {
			// ”²‚¯Šk
			{enemy::BIG_EMPTY, {_position, BIG_EMPTY_WIDTH / 2, BIG_EMPTY_WIDTH / 2, 0, BIG_EMPTY_HEIGHT / 2, true}},
			// 
		};
	}

	void BigDoll::Process() {

	}

	void BigDoll::Draw() {

	}


	void BigDoll::HipDrop() {

	}
}