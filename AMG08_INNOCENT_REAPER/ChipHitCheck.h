/*****************************************************************//**
 * @file   ChipHitCheck.h
 * @brief  マップチップの登録情報を管理するチップヒットチェッククラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "AABB.h"

namespace inr {
	/** 二重インクルード防止 */
	class ChipNumber;
	/** マップチップの当たり判定を管理する */
	class ChipHitCheck {
	public:
		/** チップの判定情報を管理する連想配列(キー:チップ番号　値:対応するチップの判定情報) */
		using ChipsMap = std::unordered_map<int, ChipNumber>;
		/**
		 * @brief	コンストラクタ
		 */
		ChipHitCheck();
		/**
		 * @brief	デストラクタ
		 */
		~ChipHitCheck();
		/**
		 * @brief						マップチップの当たり判定登録
		 * @param key				登録用キー
		 * @param chipsMap	チップ情報を格納した連想配列
		 */
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		/**
		 * @brief					取り出し用キー情報の更新
		 * @param nextkey	切り替え用キー
		 */
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		/**
		 * @brief			対象のマップチップに効果があるかの判定
		 * @param no	判定を行うチップ番号
		 * @return		対応する効果番号を返す
		 *						チップ番号が該当なしの場合は-1を返す
		 */
		int IsChipType(const int no);
		/**
		 * @brief			対象に当たり判定があるかの判定
		 * @param no	判定を行うチップ番号
		 * @return		判定がある場合はtrueを返す
		 *						判定がない場合はfalseを返す
		 */
		bool IsHitType(const int no);
		/**
		 * @brief			対象の当たり判定ボックスを取得
		 * @param no	判定を行うチップ番号
		 * @return		ヒットした場合は取得した当たり判定ボックスを返す
		 *						登録されていない場合は40*40の当たり判定ボックスを返す
		 */
		AABB ChipCollision(const int no);
	private:
		/** マップチップの判定情報を管理する連想配列(キー:チップキー　値:マップチップの判定情報を管理するコンテナ) */
		using StageMaps = std::unordered_map<std::string, ChipsMap>;
		std::string _chipKey;				//!< 取り出すマップチップ情報(キー)
		StageMaps _stageChipsMap;		//!< ステージの情報
		/**
		 * @brief	登録情報の初期化
		 */
		void ClearStageMaps();
	};
}