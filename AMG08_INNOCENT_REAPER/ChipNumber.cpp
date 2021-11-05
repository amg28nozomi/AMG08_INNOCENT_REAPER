/*****************************************************************//**
 * @file   ChipNumber.cpp
 * @brief  マップチップの登録情報を保持するチップ番号クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ChipNumber.h"

namespace inr {

  ChipNumber::ChipNumber(int width1, int width2, int height1, int height2, int type, bool isHit) {
    // 各種初期化
    widthMin = width1;
    widthMax = width2;
    heightMin = height1;
    heightMax = height2;
    chipType = type;
    hitType = isHit;
  }

  ChipNumber::ChipNumber(int width1, int width2, int type, bool isHit) {
    widthMin = width1;
    widthMax = width2;
    heightMin = 0;
    heightMax = 40;
    chipType = type;
    hitType = isHit;
  }

  ChipNumber::ChipNumber(int type, bool isHit) {
    // 各種初期化
    widthMin = 0;
    widthMax = 40;
    heightMin = 0;
    heightMax = 40;
    chipType = type;
    hitType = isHit;
  }
}