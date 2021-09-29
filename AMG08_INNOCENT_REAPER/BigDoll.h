#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;

		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;

		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";	// �|�ꂽ�ꍇ
			constexpr auto SE_ESCAP_VOICE = "big_escape";
			constexpr auto SE_TACKLE = "big_tackle";
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";

		}
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
		double _atkVec;	// �ړ���
		int _moveCount;	// �ړ��J�E���g

		void HipDrop();	// �q�b�v�h���b�v
		void StateUpdate();	// ��Ԃɉ������������s��

		void PatrolOn() override;
		void AttackOn() override;
		void EscapeOn() override;
		void Action() override;
		void Death() override;
		void ChangeIdol(int stay) override;
		void Move();	// �ړ�����
		void PositionUpdate() override;
		void Attack();	// �U��

		AABB NowCollision(std::string key) override;
	};
}

