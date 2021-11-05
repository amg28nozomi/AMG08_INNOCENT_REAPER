/*****************************************************************//**
 * @file   MapDataManager.cpp
 * @brief  マップ情報の管理を行うマップデータマネージャ
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "MapDataManager.h"
#include "MapData.h"

namespace inr {

  MapDataManager::MapDataManager(Game& game) : _game(game) {
    StageMapClear();
  }

  MapDataManager::~MapDataManager() {
    StageMapClear();
  }

  void MapDataManager::LoadStageMap(JsonMapData& jsonMapData) {
    // キーと値の取り出し
    for (auto&& j : jsonMapData) {
      auto it = _maps.find(j.first);
      if (it != _maps.end()) continue;  // 既に登録されている場合は処理をスキップ
      _maps.emplace(j.first, j.second); // 登録
    }
  }

  bool MapDataManager::GetStageMap(const std::string stage, MapData& mapData) {
    auto smap = _maps.find(stage);
    if (smap == _maps.end()) return false; // 指定したマップ情報は存在しない
    mapData = smap->second; // 取り出した情報を代入する
    return true;            // 読み込み成功
  }

  void MapDataManager::StageMapClear() {
    _maps.clear();
  }

  bool MapDataManager::IsLoad(const std::string key) {
    auto it = _maps.find(key);
    if (it == _maps.end()) return true; // 登録されていない
    return false;                       // 登録されている
  }
}