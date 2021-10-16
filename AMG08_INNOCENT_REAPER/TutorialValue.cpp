#include "TutorialValue.h"

namespace inr {
	// �R���X�g���N�^
	TutorialValue::TutorialValue() : _ivalue() {
		// �R���e�i�̏�����
		_width.clear();
		_height.clear();
	}
	// �R���X�g���N�^
	TutorialValue::TutorialValue(ImageValue ivalue, int width, int height) : _ivalue(ivalue) {
		// ������
		_width.emplace_back(width);
		_height.emplace_back(height);
	}
	// �R���X�g���N�^
	TutorialValue::TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2) : _ivalue(ivalue) {
		// ������
		_width = { width1, width2 };
		_height = { height1, height2 };
	}
}
