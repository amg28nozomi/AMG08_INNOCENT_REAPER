/*****************************************************************//**
 * @file   ChipNumber.h
 * @brief  �}�b�v�`�b�v�̓o�^����ێ�����`�b�v�ԍ��N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
	/** �}�b�v�`�b�v�̃R���p�C�����萔 */
	namespace mapchip {
		// �}�b�v�`�b�v�ɓ�����ʂ͂��邩�H
		constexpr auto NONE = -1;				//!< NULL
		constexpr auto NORMAL = 0;			//!< ���ʂȂ��̒ʏ�`�b�v
		constexpr auto THORM = 1;				//!< �_���[�W���肠��̞��`�b�v
		constexpr auto IVY = 2;					//!< �͂ݔ��肠��̒Ӄ`�b�v
		constexpr auto TRANSITION = 3;	//!< �X�e�[�W�J�ڔ��肠��̑J�ڃ`�b�v
		// �����蔻��͂��邩�H
		constexpr auto HIT_ON = true;		//! �����蔻��E�����o�������L��
		constexpr auto HIT_OFF = false;	//! �����蔻��E�����o����������
	}
	/** �}�b�v�`�b�v�̓����蔻�� */
	class ChipNumber {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 * @param width1		�����蔻��E����(min)
		 * @param width2		�����蔻��E����(max)
		 * @param height1		�����蔻��E�c��(min)
		 * @param height2		�����蔻��E�c��(max)
		 * @param type			�}�b�v�`�b�v�̌��ʔԍ�
		 * @param ishit			�}�b�v�`�b�v�ɓ�����E�����o��������������邩
		 */
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	�R���X�g���N�^
		 * @param width1	�����蔻��E����(min)
		 * @param width2	�����蔻��E����(max)
		 * @param type		�}�b�v�`�b�v�̌��ʔԍ�
		 * @param ishit		�}�b�v�`�b�v�ɓ�����E�����o��������������邩
		 */
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	�R���X�g���N�^
		 * @param type		�}�b�v�`�b�v�̌��ʔԍ�
		 * @param ishit		�}�b�v�`�b�v�ɓ�����E�����o��������������邩
		 */
		ChipNumber(int type, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	 ����(min)�̎擾
		 * @return ����(min)��Ԃ�
		 */
		inline int WidthMin() { return widthMin; }
		/**
		 * @brief	 ����(max)�̎擾
		 * @return ����(max)��Ԃ�
		 */
		inline int WidthMax() { return widthMax; }
		/**
		 * @brief	 �c��(min)�̎擾
		 * @return �c��(min)��Ԃ�
		 */
		inline int HeightMin() { return heightMin; }
		/**
		 * @brief	 �c��(max)�̎擾
		 * @return �c��(max)��Ԃ�
		 */
		inline int HeightMax() { return heightMax; }
		/**
		 * @brief		���ʔԍ��̎擾
		 * @return	���ʔԍ��̕Ԃ�
		 */
		inline int ChipType() { return chipType; }
		/**
		 * @brief		����t���O�̎擾
		 * @return	����t���O��Ԃ�
		 */
		inline bool HitType() { return hitType; }
	private:
		int widthMin;		//!< ����(Min)
		int widthMax;		//!< ����(Max)
		int heightMin;	//!< �c��(Min)
		int heightMax;	//!< �c��(Max)
		int chipType;		//!< �`�b�v�̌��ʔԍ�
		bool hitType;		//!< ������E�����o������̗L��
	};
}
