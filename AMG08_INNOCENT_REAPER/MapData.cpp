#include "MapData.h"
#include <vector>
#include <tuple>
#include <string>

namespace {
	constexpr auto NULL_DATA = -1;
	constexpr auto NULL_FILE = "";
}

namespace inr {

	MapData::MapData() {
		_mapSizeW = NULL_DATA;
		_mapSizeH = NULL_DATA;
		_chipCount = NULL_DATA;
		_chipCountW = NULL_DATA;
		_chipCountH = NULL_DATA;
		_chipSizeW = NULL_DATA;
		_chipSizeH = NULL_DATA;
		_mapSizeLayer = NULL_DATA;
		_fileName = NULL_FILE;
		_mapDatas.clear();
		_chipType.clear();
	}

	MapData::MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType) {
		_mapSizeW = mapSize.first;
		_mapSizeH = mapSize.second;
		_chipCount = std::get<0>(chipCount);
		_chipCountW = std::get<1>(chipCount);
		_chipCountH = std::get<2>(chipCount);
		_chipSizeW = chipSize.first;
		_chipSizeH = chipSize.second;
		_mapSizeLayer = mapSizeLayer;
		_fileName = fileName;
		_mapDatas.swap(mapData);
		_chipType.swap(chipType);
	}

	void MapData::SetMapSize(int mapSizeW, int mapSizeH) {
		_mapSizeW = mapSizeW;
		_mapSizeH = mapSizeH;
	}
	
	void MapData::SetChipCount(int count, int width, int height) {
		_chipCount = count;
		_chipCountW = width;
		_chipCountH = height;
	}

	void MapData::SetChipSize(int width, int height) {
		_chipSizeW = width;
		_chipSizeH = height;
	}


	MapDataManager::MapDataManager(Game& game) : _game(game) {
		StageMapClear();
	}

	MapDataManager::~MapDataManager() {
		StageMapClear();
	}

	void MapDataManager::LoadStageMap(JsonMapData& jsonMapData) {
		for (auto&& j : jsonMapData) {
			auto it = _maps.find(j.first);
			if (it != _maps.end()) continue;	// �������X�L�b�v
			_maps.emplace(j.first, j.second);	// �o�^
		}
	}

	bool MapDataManager::GetStageMap(const std::string stage, MapData& mdata) {
		// �ǂݎ�肽���f�[�^�͓o�^����Ă��邩�H
		auto smap = _maps.find(stage);
		if (smap == _maps.end()) {
			return false;	// �o�^����Ă��Ȃ��ꍇ�͏����𔲂���
		}
		mdata = smap->second;	// ���o��������������
		return true;
	}

	void MapDataManager::StageMapClear() {
		_maps.clear();
	}

	bool MapDataManager::IsLoad(const std::string key) {
		// �Ώۂ̃L�[�͓o�^����Ă��邩�H
		auto it = _maps.find(key);
		if (it == _maps.end()) return true;	// �o�^����Ă��Ȃ�
		return false;	// �o�^����Ă���
	}
}