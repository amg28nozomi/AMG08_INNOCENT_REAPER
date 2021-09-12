#include "CrowDoll.h"

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::CROW_DOLL;

	}

	void CrowDoll::Process() {

	}

	void CrowDoll::Draw() {

	}
}
