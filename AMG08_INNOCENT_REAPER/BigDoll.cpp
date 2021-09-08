#include "BigDoll.h"

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {

	}

	BigDoll::~BigDoll() {

	}

	void BigDoll::Init() {
		_mainCollision = { _position, 160 / 2, 200 / 2, true };
	}

	void BigDoll::Process() {

	}

	void BigDoll::Draw() {

	}
}