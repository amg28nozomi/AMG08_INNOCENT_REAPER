/*****************************************************************//**
 * @file   StageTransition.h
 * @brief  ステージ遷移情報の管理を行うサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {
	/** ステージのコンパイル時定数 */
	namespace stage {
		// ステージ読み込み用のキー
		constexpr auto CHANGE_NULL = "change_null";	//!< 通常状態のキー切り替え
		constexpr auto STAGE_0 = "stage0";					//!< 最初
		constexpr auto STAGE_T = "stage0-2";				//!< チュートリアル
		constexpr auto STAGE_1 = "stage1";					//!< ステージ1(廃村)
		constexpr auto STAGE_2 = "stage2";					//!< ステージ2(森林-中央)
		constexpr auto STAGE_2_1 = "stage2-1";			//!< ステージ2(森林-上部)
		constexpr auto STAGE_2_2 = "stage2-2";			//!< ステージ2(森林-下部)
		constexpr auto STAGE_3 = "stageb";					//!< ステージ3(ボス戦)
	}
	/** 二重インクルード防止 */
	class Game;
	class Transition;
	// ステージ遷移情報のサーバ
	class StageTransition {
	public:
		/**
		 * @brief			コンストラクタ
		 * @param			ゲームクラスの参照
		 */
		StageTransition(Game& game);
		/**
		 * @brief			デストラクタ
		 */
		~StageTransition();
		/**
		 * @brief			初期化処理
		 * @return		trueを返す
		 */
		bool Init();
		/**
		 * @brief			遷移チップと衝突したのかの判定
		 * @param no	チップ番号
		 * @return		接触している場合はtrueを返す
		 *						接触していない場合はfalseを返す
		 */
		bool IsHit(const int no);
		/**
		 * @brief			遷移処理を行うかの判定
		 * @return		遷移処理を行う場合はtrueを返す
		 *						更新が入っていない場合はfalseを返す
		 */
		bool IsStageChange();
		/**
		 * @brief			遷移後情報の取得
		 * @return		左辺:遷移後の座標　右辺:向き
		 */
		std::pair<Vector2, bool> SetPosition();
	private:
		Game& _game;																				//!< ゲーム
		std::unordered_map<int, Transition> _transitions;		//!< ステージ遷移情報用コンテナ
		int _number;																				//!< 接触したチップ
		/**
		 * @brief			コンテナの解放
		 */
		void Clear();
	};
}

