#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;
		// constexpr auto BIG_IMAGE = 340;

		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;

		// SE�p�L�[
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";	// �|�ꂽ�ꍇ
			constexpr auto SE_ESCAPE_VOICE = "big_escape";
			constexpr auto SE_TACKLE = "big_tackle";
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";

		}
	}

	// �r�b�O�h�[��(�G)
	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();

		void Init() override;
		void Process() override;

		void SetParameter(ObjectValue objValue);	// �I�u�W�F�N�g���̓o�^
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;	// ���@�A�N�V�����Ƃ̏Փ˔���
	private:
		double _atkVec;	// �ړ���
		int _moveCount;	// �ړ��J�E���g

		void HipDrop();	// �q�b�v�h���b�v
		void StateUpdate();	// ��Ԃɉ������������s��

		void PatrolOn() override;	// ���G��������I��
		void AttackOn() override;	// �U����������I��
		void EscapeOn() override;	// ������������I��
		void Action() override;	// �A�N�V������ԂɈڍs���邩
		void Death() override;	// ���S����
		void ChangeIdol(int stay) override;	// �ҋ@��Ԃւ̑J��
		void Move();	// �ړ�����
		void PositionUpdate() override;	// ���W�X�V
		void Attack();	// �U��

		AABB NowCollision(std::string key) override;	// ���݂̓����蔻��̎擾
	};
}

