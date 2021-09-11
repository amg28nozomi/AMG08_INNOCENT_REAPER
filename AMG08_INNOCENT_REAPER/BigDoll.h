#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;

		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;
	}

	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();

		void Init() override;
		void Process() override;

		void SetParameter(ObjectValue objValue);
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	private:
		double _atkVec;	// 移動量

		void HipDrop();	// ヒップドロップ
		void StateUpdate();	// 状態に応じた処理を行う

		void PatrolOn() override;
		void AttackOn() override;
		void EscapeOn() override;
		void Death() override;
		void ChangeIdol() override;
		void Move();	// 移動処理
		void PositionUpdate() override;
		void Attack();	// 攻撃

		AABB NowCollision(std::string key) override;
	};
}

