/*****************************************************************//**
 * @file   GimmickValue.cpp
 * @brief   ギミック情報クラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "GimmickValue.h"

namespace inr {

  GimmickValue::GimmickValue(int type, int flag, int cType, std::vector<int> types) {
    _gimmickType = type;
    _gimmickFlag = flag;
    _crystalType = cType;
    _dtype = types;
  }
}