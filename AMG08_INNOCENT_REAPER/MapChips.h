#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"
#include "MapDataManager.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "FileData.h"
#include "StageTransition.h"

namespace inr {
	/** ステージのコンパイル時定数 */
	namespace stage {
		// マップチップの種類
		constexpr auto KEY_NORMAL = "mapchip_normal";		//!< 通常マップチップ
	}
	/** マップチップのコンパイル時定数 */
	namespace mapchip {
		// マップチップのタイプ番号
		constexpr auto TYPE_NULL = -1;									//!< 未登録
		constexpr auto TYPE_NORMAL = 0;									//!< 通常チップ
		constexpr auto TYPE_THORM = 1;									//!< 棘チップ
		constexpr auto TYPE_IVX = 2;										//!< 蔦チップ
	}
	/** 二重インクルード防止 */
	class Game;
	class ObjectBase;
	/** マップチップ */
	class MapChips {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 */
		MapChips(Game& game);
		/**
		 * @brief						デストラクタ
		 */
		~MapChips();
		/**
		 * @brief						初期化処理
		 */
		void Init();
		/**
		 * @brief						更新処理
		 */
		void Process();
		/**
		 * @brief						描画処理
		 */
		void Draw();
		/**
		 * @brief						チップ番号の取得
		 * @param	x					x軸の座標(マップチップ換算)
		 * @param	y					y軸の座標(マップチップ換算)
		 * @return					マップチップがある場合は該当するチップ番号を返す
		 *									マップチップがない場合は0を返す
		 */
		int CheckHit(int x, int y);
		/**
		 * @brief						マップチップの上に立っているかの判定
		 * @param box				対象の当たり判定ボックス
		 * @param pos				対象の座標
		 * @param g					重力値
		 * @param lastpos		最後に立っていた座標
		 * @param flag			自機かどうかのフラグ
		 * @return					接触している場合は対象の効果番号を返す
		 *									接触していない場合は-1を返す
		 */
		int IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos, bool flag = false);
		/**
		 * @brief						マップチップとの接触判定(敵用)
		 * @param box				対象の当たり判定ボックス
		 * @param pos				対象の座標
		 * @param move			対象の移動量
		 * @param direction	対象の向きフラグ
		 * @return					押し出し処理を行った場合は接触しているマップチップの効果番号を返す
		 *									接触していない場合は-1を返す
		 */
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction);
		/**
		 * @brief						マップチップとの接触判定(敵用)
		 * @param box				対象の当たり判定ボックス
		 * @param pos				対象の座標
		 * @param move			対象の移動量
		 * @param direction	対象の向きフラグ
		 * @param	isGran		掴みフラグ
		 * @return					押し出し処理を行った場合は接触しているマップチップの効果番号を返す
		 *									接触していない場合は-1を返す
		 */
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool* isGran);
		/**
		 * @brief						蔦との接触判定
		 * @param box				対象の当たり判定ボックス
		 * @param pos				対象の座標
		 * @param move			対象の移動量
		 * @param ivx				蔦チップの座標を格納するポインタ
		 * @param direction	対象の向きフラグ
		 * @return 
		 */
		bool HitIvy(AABB box, Vector2 pos, Vector2 move, std::pair<double, double>* ivx, bool direction);
		/**
		 * @brief						ワールド座標をスクリーン座標にクランプする
		 * @param	pos				ワールド座標
		 * @return					クランプに成功した場合はtrueを返す	
		 *									クランプに失敗した場合はfalseを返す
		 */
		bool Clamp(Vector2& pos);
		/**
		 * @brief						横スクロールを行うかの判定
		 * @return					横スクロールを行う場合はtrueを返す
		 *									行わない場合はfalseを返す
		 */
		bool IsScrollX();
		/**
		 * @brief						縦スクロールを行うかの判定
		 * @return					縦スクロールを行う場合はtrueを返す
		 *									行わない場合はfalseを返す
		 */
		bool IsScrollY();
		/**
		 * @brief						マップチップの切り替え処理
		 * @param						次のステージキー
		 */
		void ChangeMap(std::string nextStage);
		/**
		 * @brief						マップサイズ(幅)の取得
		 * @return					現在のマップのマップサイズ(幅)を返す
		 */
		inline int GetMapSizeWidth() { return _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth(); }
		/**
		 * @brief						マップサイズの取得
		 * @return					現在のマップのマップサイズを返す
		 */
		inline std::pair<int, int> GetMapSize() { return std::make_pair(_nowMap.MapSizeWidth(), _nowMap.MapSizeHeight()); }
		/**
		 * @brief						チップカウントの取得
		 * @return					現在のマップのチップカウントを返す
		 */
		inline std::tuple<int, int, int> GetChipCount() { return std::make_tuple(_nowMap.ChipCount(), _nowMap.ChipCountWidth(), _nowMap.ChipCountHeight()); }
		/**
		 * @brief						マップチップサイズの取得
		 * @return					現在のマップのマップチップサイズを返す
		 */
		inline std::pair<int, int> GetChipSize() { return std::make_pair(_nowMap.ChipSizeWidth(), _nowMap.ChipSizeHeight()); }
		/**
		 * @brief						レイヤーの取得
		 * @return					現在のマップのレイヤーを返す
		 */
		inline int GetMapSizeLayer() { return _nowMap.MapSizeLayer(); }
		/**
		 * @brief						ファイル名の取得
		 * @return					現在のマップのファイル名を返す
		 */
		inline std::string GetFileChips() { return _nowMap.FileName(); }
		/**
		 * @brief						ワールド座標の取得
		 * @return					ワールド座標を返す
		 */
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		/**
		 * @brief						ワールド座標の移動量を取得
		 * @return					現在のフレームと前フレームのワールド座標の差分を返す
		 */
		inline Vector2 BeforeWorldPos() { return _worldPosition - _worldLast; }
		/**
		 * @brief						ステージ遷移システムの取得
		 * @return					ステージ遷移システムを返す
		 */
		inline std::unique_ptr<StageTransition>& GetStageTransition() { return _stageTransition; }
		/**
		 * @brief						ワールド座標の更新
		 * @param	pos				座標
		 */
		void WorldUpdate(Vector2 pos);
	private:
		Game& _game;																				//!< ゲームクラスの参照
		MapData _nowMap;																		//!< 現在のマップチップ情報
		std::unique_ptr<MapDataManager> _mapManager;				//!< マップ情報のマネージャー
		std::unique_ptr<ChipHitCheck> _chipCheck;						//!< マップチップの当たり判定管理用マネージャー
		std::unique_ptr<StageTransition> _stageTransition;	//!< ステージ遷移用システム
		std::string _skey;																	//!< 現在のステージ
		std::string _chipKey;																//!< 取り出すマップチップのキー
		Vector2 _worldPosition;															//!< ワールド座標
		Vector2 _worldLast;																	//!< 前フレームのワールド座標
		/**
		 * @brief							ワールド座標を修正するかの判定
		 */
		void WorldClanp();
		/**
		 * @brief							Jsonファイルの展開
		 * @param fileName		Jsonファイル名
		 * @return						展開に成功した場合は文字列を返す
		 *										失敗した場合は""を返す
		 */
		std::string StringFileLoad(std::string fileName);
		/**
		 * @brief							マップデータの読み込み
		 * @param skey				登録用キー
		 * @param filePath		ファイルパス
		 * @param strFileName	ファイル名
		 * @return						読み込みに成功した場合はtrueを返す
		 *										失敗した場合は0を返す
		 */
		int TiledJsonLoad(std::string skey, std::string filePath, std::string strFileName);
		/**
		 * @brief							各ステージの一部マップチップの判定範囲を修正
		 */
		void SetChipsMap();
		/**
		 * @brief							遷移チップと接触したかの判定
		 * @param no					チップ番号
		 * @return						接触した場合はtrueを返す
		 *										接触していない場合はfalseを返す
		 */
		bool TransitionResearch(const int no);
	};
}

