/*****************************************************************//**
 * @file   Transition.cpp
 * @brief  �X�e�[�W�J�ڏ��
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Transition.h"

namespace inr {
	// �R���X�g���N�^
	Transition::Transition(std::string stageKey, Vector2 position, bool direction) {
		_nextKey = stageKey;
		_position = position;
		_direction = direction;
	}
}
