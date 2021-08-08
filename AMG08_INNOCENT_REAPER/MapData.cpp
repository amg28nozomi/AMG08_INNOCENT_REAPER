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
		_maoDatas.swap(mapData);
		_chipType.swap(chipType);
	}
}