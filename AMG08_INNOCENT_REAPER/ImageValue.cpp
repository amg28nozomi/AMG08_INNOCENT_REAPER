/*****************************************************************//**
 * @file   ImageValue.cpp
 * @brief  �C���[�W�摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace inr {
	// �R���X�g���N�^
	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";	// �L�[�̏�����
	}
	// �R���X�g���N�^
	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// �L�[�̏�����
	}
}