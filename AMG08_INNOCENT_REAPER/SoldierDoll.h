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

		// ���Ō��菈��
		void SetStatus(Vector2 spawn, std::string soulcolor) override {
			_position = spawn;
			if (soulcolor == "red") {
				_sState = SoulState::RED;
				_aState = ActionState::IDOL;
				_divKey.first = enemy::red::SOLDIER_IDOL;
			}
			else if (soulcolor == "blue") {
				_sState = SoulState::BLUE;
				_aState = ActionState::IDOL;
				_divKey.first = enemy::blue::SOLDIER_IDOL;
			}
			else if (soulcolor == "empty") {
				_sState = SoulState::EMPTY;
				_aState = ActionState::EMPTY;
				_divKey.first = enemy::SOLDIER_EMPTY;
			}
		}
	private:
		int _aInterval;	// �A�N�V�����C���^�[�o��
		double _patrolX;
		bool _drawStop;	// �`��X�V��~

		void Move() override;
		void Action() override;
		// void AnimationInit() override;

		void Patrol();	// ����
		void PatrolOn();	// �����ԂɈڍs

		void PositionUpdate();
		void StateUpdate();	// ��Ԃ̐؂�ւ�

		// ���\�͈͂̎Z�o
		AABB VitalPart(Collision& col);
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

