#include "Door.h"

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
	}

	void Door::Init() {

	}

	void Door::Process() {

	}

	void Door::SetParameter(Vector2 spwan, std::string key) {
		_position = spwan;
		_divKey.first = key;
	}
}