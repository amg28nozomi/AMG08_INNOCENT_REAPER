/*****************************************************************//**
 * @file   BigDoll.h
 * @brief  �r�b�O�h�[���N���X�i�G�l�~�[�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	/** �G�̃R���p�C�����萔 */
	namespace enemy {
		/** �r�b�O�h�[���̉摜�T�C�Y */
		constexpr auto BIG_IMAGE = 340;
		/** �����蔻��(����) */
		constexpr auto BIG_WIDTH = 160;
		/** �����蔻��(����) */
		constexpr auto BIG_HEIGHT = 200;
		/** �r�b�O�h�[���̃R���p�C�����萔 */
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";									//!< �_�E����SE
			constexpr auto SE_ESCAPE_VOICE = "big_escape";				//!< �������{�C�X
			constexpr auto SE_TACKLE = "big_tackle";							//!< �^�b�N��SE
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";	//!< �^�b�N�����{�C�X
		}
	}
	/** �r�b�O�h�[��(�G) */
	class BigDoll : public EnemyBase {
	public:
		/**
		 * @brief							�R���X�g���N�^
		 * @param	game				�Q�[���N���X�̎Q��
		 */
		BigDoll(Game& game);
		/**
		 * @brief							�f�X�g���N�^
		 */
		~BigDoll();
		/**
		 * @brief							����������
		 */
		void Init() override;
		/**
		 * @brief							�X�V����
		 */
		void Process() override;
		/**
		 * @brief							�I�u�W�F�N�g���̓o�^
		 * @param	objValue		�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue);
		/**
		 * @brief	���@�A�N�V�����{�b�N�X�Ƃ̏Փ˔���
		 * @param	ckey				�Ώۂ̌��݂̏��(�L�[)
		 * @param	acollision	�Ώۂ̓����蔻��{�b�N�X
		 * @param	direction		�Ώۂ̌����t���O
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	private:
		int _moveCount;				//!< �ړ��J�E���g
		double _atkVec;				//!< �ړ���
		/**
		 * @brief							�q�b�v�h���b�v
		 */
		void HipDrop();
		/**
		 * @brief							��Ԃɉ����������̍X�V
		 */
		void StateUpdate();
		/**
		 * @brief							���񏈗�����I��
		 */
		void PatrolOn() override;
		/**
		 * @brief							�U����������I��
		 */
		void AttackOn() override;
		/**
		 * @brief							������������I��
		 */
		void EscapeOn() override;
		/**
		 * @brief							�A�N�V������ԂɈڍs���邩
		 */
		void Action() override;
		/**
		 * @brief							���S����
		 */
		void Death() override;
		/**
		 * @brief							�ҋ@��Ԃ̐ݒ�
		 * @param	stay				�ҋ@����
		 */
		void ChangeIdol(int stay) override;
		/**
		 * @brief							�ړ�����
		 */
		void Move();
		/**
		 * @brief							���W�X�V
		 */
		void PositionUpdate() override;
		/**
		 * @brief							�U������
		 */
		void Attack();
		/**
		 * @brief							���݂̓����蔻��̎擾
		 * @param	key					���݂̏�Ԃ�\���L�[
		 * @return						�����k�ł͂Ȃ��ꍇ�͒ʏ�̓����蔻���Ԃ�
		 *										�����k�̏ꍇ�͔����k�p�̓����蔻���Ԃ�
		 */
		AABB NowCollision(std::string key) override;
	};
}

