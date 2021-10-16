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
		auto smap = _maps.find(stage);					// キーは登録されているか
		if (smap == _maps.end()) return false;	// 指定したマップ情報は存在しない
		mdata = smap->second;										// 取り出した情報を代入する
		return true;														// 読み込み成功
	}
	// コンテナの解放
	void MapDataManager::StageMapClear() {
		_maps.clear();
	}
	// 読み込み処理を行うかの判定
	bool MapDataManager::IsLoad(const std::string key) {
		auto it = _maps.find(key);					// キーは登録されているか
		if (it == _maps.end()) return true;	// 登録されていない
		return false;												// 登録されている
	}
}
