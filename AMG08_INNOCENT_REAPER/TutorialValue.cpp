/*****************************************************************//**
 * @file   TutorialValue.cpp
 * @brief  �`���[�g���A���摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "TutorialValue.h"

namespace inr {

	TutorialValue::TutorialValue() : _ivalue() {
		// �R���e�i�̏�����
		_width.clear();
		_height.clear();
	}

	TutorialValue::TutorialValue(ImageValue ivalue, int width, int height) : _ivalue(ivalue) {
		// ������
		_width.emplace_back(width);
		_height.emplace_back(height);
	}

	TutorialValue::TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2) : _ivalue(ivalue) {
		// ������
		_width = { width1, width2 };
		_height = { height1, height2 };
	}
}
