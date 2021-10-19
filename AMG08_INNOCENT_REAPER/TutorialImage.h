/*****************************************************************//**
 * @file   TutorialImage.h
 * @brief  �`���[�g���A���摜�i�p�[�e�B�N���摜�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"
#include "AABB.h"

namespace inr {
	/** �`���[�g���A���e�L�X�g�̃R���p�C�����萔 */
	namespace tutorial {
		constexpr auto ROB = "titorial_rob";									//!< �D��
		constexpr auto GIVE = "titorial_give";								//!< �^����
		constexpr auto DASH = "titorial_dash";								//!< �_�b�V��
		constexpr auto JUMP = "titorial_jump";								//!< �W�����v
		constexpr auto GRAB = "titorial_grab";								//!< �͂�
		constexpr auto INTERACTION = "titorial_interaction";	//!< ���ׂ�
		constexpr auto CRYSTAL = "titorial_crystal";					//!< ����
		constexpr auto LEVER = "titorial_lever";							//!< ���o�[
		constexpr auto EMPTY = "titorial_empty";							//!< �����k
	}
	/** ��d�C���N���[�h�h�~ */
	class TutorialValue;
	/** �`���[�g���A���摜 */
	class TutorialImage : public Particle_Image {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 * @param game		�Q�[���N���X�̎Q��
		 */
		TutorialImage(Game& game);
		/**
		 * @brief					�f�X�g���N�^
		 */
		~TutorialImage() = default;
		/**
		 * @brief					�X�V����
		 */
		void Process() override;
		/**
		 * @brief					�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief					�p�����[�^�̐ݒ�
		 * @param tvalue	�`���[�g���A�����
		 */
		void SetTutorialImage(TutorialValue tvalue);
	private:
		bool _isCol;			//!< �����蔻��ɂ���ĕ`����s�����H
		AABB _collision;	//!< �����蔻��
		/**
		 * @brief					���@�ƐڐG���Ă��邩�̔���
		 */
		void IsCollision();
	};
}

