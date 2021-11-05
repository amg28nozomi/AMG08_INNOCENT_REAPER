/*****************************************************************//**
 * @file   Image.cpp
 * @brief  画像のスーパークラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Image.h"
#include "Vector2.h"

namespace inr {

  Image::Image(Game& game) : _game(game) {
    _graphKey = "";
    _pos = { 960, 540 };
  }

  void Image::Init() {
  }

  void Image::Process() {
  }

  void Image::Draw() {
  }

  void Image::FlagOn() {
  }
}