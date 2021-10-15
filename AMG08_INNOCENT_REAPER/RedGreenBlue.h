/*****************************************************************//**
 * @file   RedGreenBlue.h
 * @brief  RedGreenBlue�J���[���f���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {

	namespace rgb {
		constexpr auto ADD = true;
		constexpr auto SUB = false;

		constexpr auto MIN_BLEND = 0;
		constexpr auto MAX_BLEND = 255;
	}
	/** RedGreenBlue�J���[���f���N���X */
	class RedGreenBlue {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 */
		RedGreenBlue();
		/**
		 * @brief	�R���X�g���N�^
		 * @param	��
		 * @param ��
		 * @param ��
		 */
		RedGreenBlue(double red, double green, double blue);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~RedGreenBlue() = default;

		inline int Red() { return  static_cast<int>(_red); }
		inline int Green() { return static_cast<int>(_green); }
		inline int Blue() { return static_cast<int>(_blue); }

		void Min();
		void Max();
		// �P�x�͍ő傩�H
		inline bool IsBrendMax() { return _red == rgb::MAX_BLEND; }
		// �P�x�͍ŏ����H

		void Update(bool type, double value);
		void Update(bool type, double rvalue, double gvalue, double bvalue);
		// ����𒴂��Ă��邩�E������������Ă��邩�i�t���O�ɉ����ĕԂ��l��؂�ւ��j
		/*bool IsMaxValue(bool type);*/
	private:
		double _red;
		double _green;
		double _blue;

		void SetBlend(double* value);
	};
}

