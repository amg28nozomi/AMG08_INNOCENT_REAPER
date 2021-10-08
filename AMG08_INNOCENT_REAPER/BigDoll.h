/*****************************************************************//**
 * \file   BigDoll.h
 * \brief  �r�b�O�h�[���N���X�i�G�l�~�[�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	namespace enemy {
		// �r�b�O�h�[���̉摜�T�C�Y
		constexpr auto BIG_IMAGE = 280;
		// �����蔻��
		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;
		// SE�����p�L�[�iSoundServer�j
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";	// �_�E��SE
			constexpr auto SE_ESCAPE_VOICE = "big_escape";	// �������{�C�X
			constexpr auto SE_TACKLE = "big_tackle";	// �^�b�N��SE
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";	// �^�b�N�����{�C�X
		}
	}
	// �r�b�O�h�[��(�G)
	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �I�u�W�F�N�g���̓o�^(����:�I�u�W�F�N�g���)
		void SetParameter(ObjectValue objValue);
		// ���@�A�N�V�����Ƃ̏Փ˔���
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	private:
		double _atkVec;	// �ړ���
		int _moveCount;	// �ړ��J�E���g
		// �q�b�v�h���b�v
		void HipDrop();
		// ��Ԃɉ����������̍X�V
		void StateUpdate();
		// ���񏈗�����I��
		void PatrolOn() override;
		// �U����������I��
		void AttackOn() override;
		// ������������I��
		void EscapeOn() override;
		// �A�N�V������ԂɈڍs���邩
		void Action() override;
		// ���S����
		void Death() override;
		// �ҋ@��Ԃ̐ݒ�
		void ChangeIdol(int stay) override;
		// �ړ�����
		void Move();
		// ���W�X�V
		void PositionUpdate() override;
		// �U������
		void Attack();
		// ���݂̓����蔻��̎擾
		AABB NowCollision(std::string key) override;
	};
}

