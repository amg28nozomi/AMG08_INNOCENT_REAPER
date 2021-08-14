#include "GimmickBase.h"
#include "Game.h"

namespace inr {

	GimmickBase::GimmickBase(Game& game) : ObjectBase(game), _gimmick(GimmickType::LEVER) {

	}

	GimmickBase::~GimmickBase() {

	}

	void GimmickBase::Init() {

	}

	void GimmickBase::Draw() {

	}
}