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

	void CrowDoll::Init() {
		_target = { 0, 0 };
		_mainCollision;
	}

	void CrowDoll::Process() {

	}

	void CrowDoll::Draw() {

	}

	void CrowDoll::WakeUp() {
		// �N���オ�胂�[�V����

	}

	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;
		_cState = nextState;
		_changeGraph = true;
	}
}
