/*****************************************************************//**
 * @file   GimmickValue.cpp
 * @brief   �M�~�b�N���N���X
 *
 * @author ��؊�C
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