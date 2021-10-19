/*****************************************************************//**
 * @file   TutorialValue.h
 * @brief  �`���[�g���A���摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ImageValue.h"
#include <vector>

namespace inr {
	/** �`���[�g���A���摜��� */
	class TutorialValue {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 */
		TutorialValue();
		/**
		 * @brief					�R���X�g���N�^
		 * @param ivalue	�摜���
		 * @param width		��
		 * @param height	����
		 */
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		/**
		 * @brief					�R���X�g���N�^
		 * @param ivalue	�摜���
		 * @param width1	��(min)
		 * @param width2	��(max)
		 * @param height1	����(min)
		 * @param height2	����(max)
		 */
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);
		/**
		 * @brief					�摜���̎擾
		 * @return				�摜����Ԃ�
		 */
		inline ImageValue GetImageValue() { return _ivalue; }
		/**
		 * @brief					���̎擾
		 * @return				����Ԃ�
		 */
		std::vector<int> Width() { return _width; }
		/**
		 * @brief					�����̎擾
		 * @return				������Ԃ�
		 */
		std::vector<int> Height() { return _height; }
	private:
		ImageValue _ivalue;				//!< �C���[�W���
		std::vector<int> _width;	//!< ��
		std::vector<int> _height;	//!< ����
	};
}

