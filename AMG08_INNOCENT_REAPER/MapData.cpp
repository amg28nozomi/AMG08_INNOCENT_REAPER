#include "MapData.h"
#include <vector>
#include <tuple>
#include <string>

namespace inr {

	MapData::MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType) {
		_mapSizeW = mapSize.first;
		_mapSizeH = mapSize.second;
		_chipCount = std::get<0>(chipCount);
		_chipCount = std::get<1>(chipCount);
		_chipCount = std::get<2>(chipCount);
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
			auto&& it = _maps.find(j.first);
			if (it == _maps.end()) continue;	// 処理をスキップ
			_maps.emplace(it->first, it->second);	// 登録
		}
	}

	bool MapDataManager::GetStageMap(const std::string stage, MapData& mdata) {
		// 読み取りたいデータは登録されているか？
		auto smap = _maps.find(stage);
		if (smap == _maps.end()) {
			return false;	// 登録されていない場合は処理を抜ける
		}
		mdata = smap->second;	// 取り出した情報を代入する
		return true;
	}

	void MapDataManager::StageMapClear() {
		_maps.clear();
	}
}