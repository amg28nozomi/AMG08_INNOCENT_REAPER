/*****************************************************************//**
 * \file   MapData.h
 * \brief  マップデータクラス
 *		   マップ情報
 * 
 *		   マップデータマネージャークラス
 *		   マップ情報の管理を行う
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

using std::pair;
using std::tuple;

namespace inr {
	// 二重インクルード防止
	class Game;
	// マップデータ
	class MapData {
	public:
		// コンストラクタ
		MapData();
		// コンストラクタ
		// 引数1:マップの長さ(幅　高さ)　引数2:チップ数(総数　横　縦)　引数3:マップチップのサイズ(幅　高さ)　引数4:レイヤー数
		// 引数5:ファイル名　引数6:マップチップの配置情報が格納されたコンテナ　引数7:マップちっぴの当たり判定が格納されたコンテナ
		MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
		// デストラクタ
		~MapData() = default;
		// マップサイズ(幅)の取得
		inline int MapSizeWidth() { return _mapSizeW; }
		// マップサイズ(高さ)の取得
		inline int MapSizeHeight() { return _mapSizeH; }
		// 総チップ数の取得
		inline int ChipCount() { return _chipCount; }
		// マップサイズの幅(チップ数)の取得
		inline int ChipCountWidth() { return _chipCountW; }
		// マップサイズの高さ(チップ数)の取得
		inline int ChipCountHeight() { return _chipCountH; }
		// マップチップの幅の取得
		inline int ChipSizeWidth() { return _chipSizeW; }
		// マップチップの高さの取得
		inline int ChipSizeHeight() { return _chipSizeH; }
		// レイヤーの取得
		inline int MapSizeLayer() { return _mapSizeLayer; }
		// ファイル名の取得
		inline std::string FileName() { return _fileName; }
		// 指定したマップチップの配置情報を取得(引数:取得したい要素)
		int MapDatas(int element) { return _mapDatas[element]; }
		// マップチップの配置情報の取得
		std::vector<int> MapDatas() { return _mapDatas; }
		// 当たり判定の取得
		std::vector<int> ChipType() { return _chipType; }
		// マップサイズの登録(引数1:幅　引数2:高さ)
		void SetMapSize(int mapSizeW, int mapSizeH);
		// 
		void SetChipCount(int count, int width, int height);
		// マップチップサイズの登録(引数1:幅　引数2:高さ)
		void SetChipSize(int width, int height);
		// レイヤーの登録(引数:レイヤー数)
		inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
		// ファイル名の登録(引数:ファイル名)
		inline void SetFillName(std::string filename) { _fileName = filename; }
		// 配置情報の登録(引数:配置情報が登録されたint型の動的配列)
		inline void SetMapDatas(std::vector<int> mapdatas) { _mapDatas = mapdatas; }
		// 当たり判定の登録(引数:当たり判定が登録されたint型の動的配列)
		inline void SetChipType(std::vector<int> chiptype) { _chipType = chiptype; }
	private:
		int _mapSizeW;					// マップサイズの幅
		int _mapSizeH;					// マップサイズ高さ
		int _chipCount;					// 総チップ数
		int _chipCountW;				// マップサイズの幅(チップ数)
		int _chipCountH;				// マップチップの高さ(チップ数)
		int _chipSizeW;					// マップチップの幅
		int _chipSizeH;					// マップチップの高さ
		int _mapSizeLayer;				// レイヤー
		std::string _fileName;			// 読み込みファイル名
		std::vector<int> _mapDatas;		// マップチップの配置情報
		std::vector<int> _chipType;		// 当たり判定があるマップチップ
	};

	// マップデータ管理用のクラス
	class MapDataManager {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		MapDataManager(Game& game);
		// デストラクタ
		~MapDataManager();
		// マップ情報を管理するコンテナ
		using JsonMapData = std::unordered_map<std::string, MapData>;

		void LoadStageMap(JsonMapData& jsonMapData);	// 登録
		bool GetStageMap(const std::string stage, MapData& mdata);	// 指定したマップデータの読み込み(引数1:読み取りたいマップのキー、引数2:値を代入するオブジェクト)
		bool IsLoad(const std::string key);	// キーは登録されているか？
		// コンテナの解放
		void StageMapClear();
	private:
		JsonMapData _maps;	// マップ情報を管理するコンテナ
		Game& _game;		// ゲームクラスの参照
	};
}

