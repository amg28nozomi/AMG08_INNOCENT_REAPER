#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"
#include "Vector2.h"

namespace inr {

	class Game;
	class ObjectBase;

	class MapChips {
	public:
		// ファイル名とjson形式が同じ場合のコンストラクタ
		MapChips(Game& game, std::string& filePath, std::string& tiledFileName);
		~MapChips();

		void Init();	// 初期化。モード変更・
		void Process();
		void Draw();

		int CheckHit(int x, int y);
		// マップチップと当たっているかどうか（マップチップの上に立っているかどうか）
		bool IsHit(AABB box, double g);	// pos:ボックス　mx:xの移動距離　my:yの移動距離
		bool IsHit(AABB box, Vector2& pos, Vector2& move);	// 押し出し処理用
		bool Clamp(Vector2& pos);	// ワールド座標をスクリーン座標に修正

		// ゲッター
		inline std::pair<int, int> GetMapSize() { return _mapSize; }
		inline std::tuple<int, int, int> GetChipCOunt() { return _chipCount; }
		inline std::pair<int, int> GetChipSize() { return _chipSize; }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFillCHips() { 
			std::string* fillchips = &_fileChips;
			return *fillchips;
		}
		// ワールド座標の取得
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		void WorldUpdate(Vector2 pos);	// ワールド座標の更新

	private:
		using MapList = std::unordered_map<std::string, MapData>;
		Game& _game;

		std::pair<int, int> _mapSize;	// マップのサイズ(チップ数　first:width、second:hight)
		std::tuple<int, int, int> _chipCount;

		std::pair<int, int> _chipSize;	// チップの描画サイズ(pixel)
		int _mapSizeLayer;	// レイヤー数
		std::string _fileChips;	// チップデータのファイル名

		Vector2 _worldPosition;	// ワールド座標
		std::vector<int> _mapData;	// マップチップの配置情報
		std::vector<int> _mapChipsType;	// 当たり判定のあるマップチップ(識別番号)登録用

		void WorldClanp();
		std::string StringFillLoad(std::string FillName);
		int TiledJsonLoad(std::string fillPath, std::string strFillName);

		MapList _maplist;


		// デバッグ用
		/*AABB _debugAABB;*/
	};
}

