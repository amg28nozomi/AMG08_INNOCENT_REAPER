#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

using std::pair;
using std::tuple;

namespace inr {

	class Game;

	class MapData {
	public:
		MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
		~MapData() = default;

		inline pair<int, int> GetMapSize() { return std::make_pair(_mapSizeW, _mapSizeH); }
		inline tuple<int, int, int> GetChipCount() { return std::make_tuple(_chipCount, _chipSizeW, _chipSizeH); }
		inline pair<int, int> GetChipSize() { return std::make_pair(_chipSizeW, _chipSizeH); }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFileName() { return _fileName; }

		void SetMapSize(int mapSizeW, int mapSizeH);
		void SetChipCount(int count, int width, int height);
		void SetChipSize(int width, int height);
		inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
		inline void SetFillName(std::string filename) { _fileName = filename; }
		inline void SetMapDatas(std::vector<int> mapdatas) { _mapDatas = mapdatas; }
		inline void SetChipType(std::vector<int> chiptype) { _chipType = chiptype; }

	private:
		int _mapSizeW;		// マップサイズの幅
		int _mapSizeH;		// マップサイズ高さ
		int _chipCount;		// 総チップ数
		int _chipCountW;	// 
		int _chipCountH;	//  
		int _chipSizeW;		// 
		int _chipSizeH;		// 
		int _mapSizeLayer;	// 
		std::string _fileName;	// 読み込みファイル名
		std::vector<int> _mapDatas;	// マップチップの配置情報
		std::vector<int> _chipType;	// 当たり判定があるマップチップ
	};

	// マップデータ管理用のクラス
	class MapDataManager {
	public:
		MapDataManager(Game& game);
		~MapDataManager();

		using JsonMapData = std::unordered_map<std::string, MapData>;

		void LoadStageMap(JsonMapData& jsonMapData);	// 登録
		bool GetStageMap(const std::string stage, MapData& mdata);	// 指定したマップデータの読み込み(引数1:読み取りたいマップのキー、引数2:値を代入するオブジェクト)
		void StageMapClear();	// 連想配列の初期化
	private:
		JsonMapData _maps;	// マップの情報を保存
		Game& _game;
	};
}

