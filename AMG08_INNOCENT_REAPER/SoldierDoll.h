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

		void SetParameter(ObjectValue objValue) override;
	private:

		void Move() override;

		void Patrol();	// ����
		void PatrolOn() override;	// �����ԂɈڍs
		void Escape();
		void EscapeOn() override;
		void AttackOn() override;	// �U���J�n
		void Attack();	// �U������

		void ChangeIdol() override;
		void PositionUpdate() override;
		void StateUpdate();	// ��Ԃ̐؂�ւ�

		void Death() override;	// ���S����

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

