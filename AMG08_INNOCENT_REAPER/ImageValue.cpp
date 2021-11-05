/*****************************************************************//**
 * @file   ImageValue.cpp
 * @brief  ƒCƒ[ƒW‰æ‘œ‚Ìî•ñ
 *
 * @author —é–ØŠóŠC
 * @date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace inr {

  ImageValue::ImageValue() : _particleValue(), _position() {
    _graph = "";
  }

  ImageValue::ImageValue(std::string gKey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
    _graph = gKey;
  }
}