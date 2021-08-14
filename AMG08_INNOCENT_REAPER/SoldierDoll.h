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
		bool _drawStop;	// 描画更新停止

		void Move() override;
		void Action() override;
		// void AnimationInit() override;

		void Patrol();	// 巡回
		void PatrolOn();	// 巡回状態に移行

		void PositionUpdate();

		// 干渉可能範囲の算出
		AABB VitalPart(Collision& col);
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

