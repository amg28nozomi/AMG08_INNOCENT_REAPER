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
		void Draw() override;

		void SetParameter(ObjectValue objValue);
	private:
		double _atkVec;	// �ړ���

		void HipDrop();	// �q�b�v�h���b�v
		void StateUpdate();	// ��Ԃɉ������������s��

		void PatrolOn() override;
		void AttackOn() override;
		void Death() override;
		void ChangeIdol() override;
		void Move();	// �ړ�����
	};
}

