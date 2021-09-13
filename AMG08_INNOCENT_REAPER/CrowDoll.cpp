#include "CrowDoll.h"

namespace {
	constexpr auto CROW_WIDTH = 60;	// 横の当たり判定
	constexpr auto CROW_HEIGHT = 120;	// 縦の当たり判定
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
		// 起き上がりモーション

	}

	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;
		_cState = nextState;
		_changeGraph = true;
	}
}
