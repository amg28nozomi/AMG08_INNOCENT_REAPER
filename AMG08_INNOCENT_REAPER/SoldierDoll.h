#pragma once
#include "EnemyBase.h"

namespace inr {

	constexpr auto SOLDIER_IMAGE_W = 160;
	constexpr auto SOLDIER_IMAGE_H = 160;

	class SoldierDoll : public EnemyBase {
	public:
		SoldierDoll(Game& game);
		~SoldierDoll() override;

		void Init() override;
		void Process() override;

		bool Hit() override;
	private:
		double _patrolX;

		void Move() override;
		void Action() override;
		// void AnimationInit() override;

		void Patrol();	// ����
		void PatrolOn();	// �����ԂɈڍs

		void PositionUpdate();
	};
}
