/*****************************************************************//**
 * @file   ObjectValue.cpp
 * @brief  オブジェクト情報クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ObjectValue.h"

namespace inr {

  ObjectValue::ObjectValue() : _gimmick(){
    _class = oscenario::OBJ_NULL;
    _spawnPos.clear();
    _soulType = 0;
    _gimmick = GimmickValue();
    _objType = IsObjectType(_class);
  }

  ObjectValue::ObjectValue(int classType, Vector2 xy, bool direction, int soulColor, GimmickValue gValue) {
    // 各種情報の登録
    _class = classType;
    _spawnPos.emplace_back(xy);
    _direction = direction;
    _soulType = soulColor;
    _gimmick = gValue;
    _objType = IsObjectType(_class);
  }

  ObjectValue::ObjectValue(int classType, std::vector<Vector2> xy, bool direction, int soulColor, GimmickValue gValue) {
    _class = classType;
    _spawnPos = xy;
    _direction = direction;
    _soulType = soulColor;
    _gimmick = gValue;
    _objType = IsObjectType(_class);
  }

  int ObjectValue::IsObjectType(int className) {
    switch (className) {
    case oscenario::OBJ_PLAYER:
      return oscenario::type::PLAYER;
    case oscenario::OBJ_SOLDIER_DOLL:
    case oscenario::OBJ_BIG_DOLL:
    case oscenario::OBJ_CROW_DOLL:
      return oscenario::type::ENEMY;
    case oscenario::OBJ_SOUL:
      return oscenario::type::SOUL;
    case oscenario::OBJ_LEVER:
    case oscenario::OBJ_BLOCK:
    case oscenario::OBJ_CRYSTAL:
      return oscenario::type::GIMMICK;
    case oscenario::OBJ_ITEM:
      return oscenario::type::ITEM;
    default:
      return -1; // バグ
    }
  }

  void ObjectValue::PositionsUpdate(Vector2 newPos) {
    _spawnPos.clear();
    _spawnPos.emplace_back(newPos);
  }

  void ObjectValue::FlagUpdate(int flag, int soul) {
    if (_gimmick.GimmickFlag() != flag) _gimmick.AddFlag(flag);
    if (soul == 0) return;
    if (_soulType != soul) _soulType = soul;
  }
}