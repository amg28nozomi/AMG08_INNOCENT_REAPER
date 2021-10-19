/*****************************************************************//**
 * @file   MapData.cpp
 * @brief  マップデータクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "MapData.h"
#include <vector>
#include <tuple>
#include <string>

namespace {
	// 初期化用
	constexpr auto NULL_DATA = -1;
	constexpr auto NULL_FILE = "";
}
namespace inr {

	MapData::MapData() {
		// 各種初期化
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
		// 各種初期化
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
}