/*****************************************************************//**
 * @file   MapDataManager.cpp
 * @brief  �}�b�v���̊Ǘ����s���}�b�v�f�[�^�}�l�[�W��
 *
 * @author ��؊�C
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
    // �L�[�ƒl�̎��o��
    for (auto&& j : jsonMapData) {
      auto it = _maps.find(j.first);
      if (it != _maps.end()) continue;  // ���ɓo�^����Ă���ꍇ�͏������X�L�b�v
      _maps.emplace(j.first, j.second); // �o�^
    }
  }

  bool MapDataManager::GetStageMap(const std::string stage, MapData& mapData) {
    auto smap = _maps.find(stage);
    if (smap == _maps.end()) return false; // �w�肵���}�b�v���͑��݂��Ȃ�
    mapData = smap->second; // ���o��������������
    return true;            // �ǂݍ��ݐ���
  }

  void MapDataManager::StageMapClear() {
    _maps.clear();
  }

  bool MapDataManager::IsLoad(const std::string key) {
    auto it = _maps.find(key);
    if (it == _maps.end()) return true; // �o�^����Ă��Ȃ�
    return false;                       // �o�^����Ă���
  }
}