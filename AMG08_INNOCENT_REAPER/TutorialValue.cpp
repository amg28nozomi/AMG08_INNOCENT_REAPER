/*****************************************************************//**
 * @file   TutorialValue.cpp
 * @brief  チュートリアル画像の情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "TutorialValue.h"

namespace inr {

  TutorialValue::TutorialValue() : _ivalue() {
    _width.clear();
    _height.clear();
  }

  TutorialValue::TutorialValue(ImageValue iValue, int width, int height) : _ivalue(iValue) {
    _width.emplace_back(width);
    _height.emplace_back(height);
  }

  TutorialValue::TutorialValue(ImageValue iValue, int width1, int width2, int height1, int height2) : _ivalue(iValue) {

    _width = { width1, width2 };
    _height = { height1, height2 };
  }
}