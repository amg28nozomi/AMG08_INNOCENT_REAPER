#include "CrowDoll.h"

namespace {
	constexpr auto CROW_WIDTH = 60;	// ���̓����蔻��
	constexpr auto CROW_HEIGHT = 120;	// �c�̓����蔻��
}

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
