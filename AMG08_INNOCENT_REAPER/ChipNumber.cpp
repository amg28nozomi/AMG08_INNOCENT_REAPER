/*****************************************************************//**
 * @file   ChipNumber.cpp
 * @brief  �}�b�v�`�b�v�̓o�^����ێ�����`�b�v�ԍ��N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ChipNumber.h"

namespace inr {
	// �R���X�g���N�^
	ChipNumber::ChipNumber(int width1, int width2, int height1, int height2, int type, bool ishit) {
		// �e�평����
		widthMin = width1;
		widthMax = width2;
		heightMin = height1;
		heightMax = height2;
		chipType = type;
		hitType = ishit;
	}
	// �R���X�g���N�^
	ChipNumber::ChipNumber(int width1, int width2, int type, bool ishit) {
		// �e�평����
		widthMin = width1;
		widthMax = width2;
		heightMin = 0;
		heightMax = 40;
		chipType = type;
		hitType = ishit;
	}
	// �R���X�g���N�^
	ChipNumber::ChipNumber(int type, bool ishit) {
		// �e�평����
		widthMin = 0;
		widthMax = 40;
		heightMin = 0;
		heightMax = 40;
		chipType = type;
		hitType = ishit;
	}
}
