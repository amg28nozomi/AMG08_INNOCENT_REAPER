/*****************************************************************//**
 * @file   ImageValue.cpp
 * @brief  �C���[�W�摜�̏��
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace inr {

	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";		// �L�[�̏�����
	}

	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// �L�[�̏�����
	}
}