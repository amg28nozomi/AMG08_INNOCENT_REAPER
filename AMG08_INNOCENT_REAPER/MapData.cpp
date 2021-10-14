/*****************************************************************//**
 * \file   MapData.cpp
 * \brief  マップデータクラス
 *		   マップ情報
 *
 *		   マップデータマネージャークラス
 *		   マップ情報の管理を行う
 *
 * \date   October 2021
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
	// コンストラクタ
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
	// コンストラクタ
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
	// マップサイズの登録
	void MapData::SetMapSize(int mapSizeW, int mapSizeH) {
		_mapSizeW = mapSizeW;
		_mapSizeH = mapSizeH;
	}
	// チップカウントの登録
	void MapData::SetChipCount(int count, int width, int height) {
		_chipCount = count;
		_chipCountW = width;
		_chipCountH = height;
	}
	// マップサイズの登録
	void MapData::SetChipSize(int width, int height) {
		_chipSizeW = width;
		_chipSizeH = height;
	}
	// コンストラクタ
	MapDataManager::MapDataManager(Game& game) : _game(game) {
		StageMapClear();	// 初期化
	}
	// コンストラクタ
	MapDataManager::~MapDataManager() {
		StageMapClear();	// コンテナの解放
	}
	// マップ情報の一括読み込み
	void MapDataManager::LoadStageMap(JsonMapData& jsonMapData) {
		// キーと値の取り出し
		for (auto&& j : jsonMapData) {
			auto it = _maps.find(j.first);		// キーは登録されているか
			if (it != _maps.end()) continue;	// 既に登録されている場合は処理をスキップ
			_maps.emplace(j.first, j.second);	// 登録
		}
	}
	// 指定したマップ情報の読み込み
	bool MapDataManager::GetStageMap(const std::string stage, MapData& mdata) {
		auto smap = _maps.find(stage);			// キーは登録されているか
		if (smap == _maps.end()) return false;	// 指定したマップ情報は存在しない
		mdata = smap->second;					// 取り出した情報を代入する
		return true;
	}
	// コンテナの解放
	void MapDataManager::StageMapClear() {
		_maps.clear();
	}
	// 読み込み処理を行うかの判定
	bool MapDataManager::IsLoad(const std::string key) {
		auto it = _maps.find(key);			// キーは登録されているか
		if (it == _maps.end()) return true;	// 登録されていない
		return false;						// 登録されている
	}
}