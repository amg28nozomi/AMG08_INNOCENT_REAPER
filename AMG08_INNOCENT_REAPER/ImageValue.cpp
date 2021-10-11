/*****************************************************************//**
 * \file   ImageValue.cpp
 * \brief  �C���[�W�o�����[�N���X
 *         �p�[�e�B�N���o�����[�N���X
 *		   �`���[�g���A���o�����[�N���X
 *		   �摜�N���X���p�������T�u�N���X�̊e��f�[�^
 * 
 * \author nozom
 * \date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace {
	constexpr auto DEFAULT_RATE = 1.0;		// �g�k��
}

namespace inr {
	// �R���X�g���N�^
	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";	// �L�[�̏�����
	}
	// �R���X�g���N�^
	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// �L�[�̏�����
	}
	// �R���X�g���N�^
	ParticleValue::ParticleValue() {
		// ������
		_animation = false;
		_particle = false;
		_exrate = DEFAULT_RATE;
	}
	// �R���X�g���N�^
	ParticleValue::ParticleValue(bool animation, bool particle, double exrate) {
		// ������
		_animation = animation;
		_particle = particle;
		_exrate = exrate;
	}
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