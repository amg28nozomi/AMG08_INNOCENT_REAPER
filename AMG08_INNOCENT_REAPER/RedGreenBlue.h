/*****************************************************************//**
 * @file   RedGreenBlue.h
 * @brief  RedGreenBlue�J���[���f���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
	/** RBG�J���[���f���̃R���p�C�����萔 */
	namespace rgb {
		constexpr auto ADD = true;				//!< ���Z
		constexpr auto SUB = false;				//!< ���Z
		constexpr auto MIN_BLEND = 0;			//!< ����
		constexpr auto MAX_BLEND = 255;		//!< ���
	}
	/** RedGreenBlue�J���[���f���N���X */
	class RedGreenBlue {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 */
		RedGreenBlue();
		/**
		 * @brief					�R���X�g���N�^
		 * @param					��
		 * @param					��
		 * @param					��
		 */
		RedGreenBlue(double red, double green, double blue);
		/**
		 * @brief					�f�X�g���N�^
		 */
		~RedGreenBlue() = default;
		/**
		 * @brief					RGB�l(��)�̎擾
		 * @return				RGB�l(��)��Ԃ�
		 */
		inline int Red() { return  static_cast<int>(_red); }
		/**
		 * @brief					RGB�l(��)�̎擾
		 * @return				RGB�l(��)��Ԃ�
		 */
		inline int Green() { return static_cast<int>(_green); }
		/**
		 * @brief					RGB�l(��)�̎擾
		 * @return				RGB�l(��)��Ԃ�
		 */
		inline int Blue() { return static_cast<int>(_blue); }
		/**
		 * @brief					RGB�l�������ɏC��
		 */
		void Min();
		/**
		 * @brief					RGB�l������ɏC��
		 */
		void Max();
		/**
		 * @brief					RGB�l�͏���ɓ��B���Ă��邩�H
		 * @return				����ɓ��B���Ă���ꍇ��true��Ԃ�
		 *								���B���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		inline bool IsBrendMax() { return _red == rgb::MAX_BLEND; }
		/**
		 * @brief					RGB�l�̈ꊇ�X�V
		 * @param type		�����^�C�v
		 * @param value		�l
		 */
		void Update(bool type, double value);
		/**
		 * @brief					RGB�l�̍X�V
		 * @param type		�����^�C�v
		 * @param rvalue	�Ԃ̒l
		 * @param gvalue	�΂̒l
		 * @param bvalue	�̒l
		 */
		void Update(bool type, double rvalue, double gvalue, double bvalue);
	private:
		double _red;			//!< ��
		double _green;		//!< ��
		double _blue;			//!< ��
		/**
		 * @brief					�w�肵��RGB�l�̏C��
		 * @param value		�C���Ώۂ̃|�C���^
		 */
		void SetBlend(double* value);
	};
}

