/*****************************************************************//**
 * @file   ParticleValue.cpp
 * @brief  パーティクル画像の情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ParticleValue.h"
#include "ImageValue.h"

namespace {
  constexpr auto DEFAULT_RATE = 1.0; // デフォルトの拡縮率
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