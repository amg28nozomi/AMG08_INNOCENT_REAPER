#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "FileData.h"

namespace inr {

	namespace stage {
		// ステージ読み込み用のキー
		constexpr auto CHANGE_NULL = "change_null";	// 通常状態のキー切り替え

		constexpr auto STAGE_0 = "stage0";	// チュートリアル
		constexpr auto STAGE_1 = "stage1";		// ステージ1(廃村)
		constexpr auto STAGE_2_1 = "stage2-1";	// ステージ2(森林-中央)
		constexpr auto STAGE_2_2 = "stage2-2";	// ステージ2(森林-上部)
		constexpr auto STAGE_2_3 = "stage2-3";	// ステージ2(森林-下部)
		constexpr auto STAGE_3 = "boss_stage";	// ステージ3(ボス戦)

		constexpr auto KEY_NORMAL = "mapchip_normal";	// 通常マップチップ
	}

	namespace mapchip {
		constexpr auto TYPE_NULL = -1;	// 未登録番号
		constexpr auto TYPE_NORMAL = 0;	// 通常
		constexpr auto TYPE_THORM = 1;	// 棘
		constexpr auto TYPE_IVX = 2;	// 蔦
	}

	class Game;
	class ObjectBase;

	class MapChips {
	public:
		MapChips(Game& game);
		// ファイル名とjson形式が同じ場合のコンストラクタ
		MapChips(Game& game, std::string& filePath, std::string& tiledFileName);
		~MapChips();

		void Init();	// 初期化。モード変更・
		void Process();
		void Draw();

		int CheckHit(int x, int y);

		int IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos);	// マップチップの上に立っているかどうか？
		// マップチップと当たっているかどうか（マップチップの上に立っているかどうか）
		// bool IsHit(AABB box, double g);	// pos:ボックス　mx:xの移動距離　my:yの移動距離
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool isUpdate);	// 押し出し処理用
		bool Clamp(Vector2& pos);	// ワールド座標をスクリーン座標に修正
		bool IsScrollX();	// 横スクロールしているか？
		bool IsScrollY();	// 縦スクロールしているか？

		void ChangeMap();	// マップの切り替え処理

		// ゲッター
		inline int GetMapSizeWidth() { return _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth(); }
		inline std::pair<int, int> GetMapSize() { return std::make_pair(_nowMap.MapSizeWidth(), _nowMap.MapSizeHeight()); }
		inline std::tuple<int, int, int> GetChipCount() { return std::make_tuple(_nowMap.ChipCount(), _nowMap.ChipCountWidth(), _nowMap.ChipCountHeight()); }
		inline std::pair<int, int> GetChipSize() { return std::make_pair(_nowMap.ChipSizeWidth(), _nowMap.ChipSizeHeight()); }
		inline int GetMapSizeLayer() { return _nowMap.MapSizeLayer(); }
		inline std::string GetFileChips() { return _nowMap.FileName(); }
		// ワールド座標の取得
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		// ワールド座標の移動量を取得
		inline Vector2 BeforeWorldPos() { return _worldPosition - _worldLast; }
		void WorldUpdate(Vector2 pos);	// ワールド座標の更新

	private:
		Game& _game;

		MapData _nowMap;	// 現在のマップチップ情報
		std::unique_ptr<MapDataManager> _mapManager;
		std::unique_ptr<ChipHitCheck> _chipCheck;

		std::string _skey;	// 現在のステージ
		std::string _nextStage;	// 次のステージ

		std::string _chipKey;	// 取り出すマップチップのキー

		std::vector<FileData> _sFiles;

		Vector2 _worldPosition;	// ワールド座標
		Vector2 _worldLast;	// 前フレームのワールド座標

		void WorldClanp();
		std::string StringFillLoad(std::string FillName);
		// 引数1:登録用キー　引数2:ファイルパス　引数3:ファイル名
		int TiledJsonLoad(std::string skey, std::string filePath, std::string strFileName);
		void SetChipsMap();	// 各ステージの一部マップチップの判定範囲を修正
		void SetChipMember(); // member

		//std::pair<int, int> _mapSize;	// マップのサイズ(チップ数　first:width、second:hight)
		//std::tuple<int, int, int> _chipCount;

		//std::pair<int, int> _chipSize;	// チップの描画サイズ(pixel)
		//int _mapSizeLayer;	// レイヤー数
		//std::string _fileChips;	// チップデータのファイル名


		// デバッグ用
		/*AABB _debugAABB;*/
	};
}

