/*****************************************************************//**
 * @file   ParticleValue.cpp
 * @brief  �p�[�e�B�N���摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ParticleValue.h"
#include "ImageValue.h"

namespace {
  constexpr auto DEFAULT_RATE = 1.0; // �f�t�H���g�̊g�k��
}
namespace inr {

  ParticleValue::ParticleValue() {
    _animation = false;
    _particle = false;
    _exrate = DEFAULT_RATE;
  }

  ParticleValue::ParticleValue(bool animation, bool particle, double exrate) {
    _animation = animation;
    _particle = particle;
    _exrate = exrate;
  }
}