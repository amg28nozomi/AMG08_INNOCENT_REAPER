/*****************************************************************//**
 * \file   ChipHitCheck.h
 * \brief  チップ番号クラス
 * \brief	 マップチップの登録情報
 * \brief	 チップヒットチェッククラス
 * \brief  マップチップの当たり判定を管理する
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {
	/** 二重インクルード防止 */
	class ChipNumber;
	/** マップチップの当たり判定を管理する */
	class ChipHitCheck {
	public:
		// チップ情報を格納する連想配列(キー:チップ番号　値:マップチップの当たり判定)
		using ChipsMap = std::unordered_map<int, ChipNumber>;
		// コンストラクタ
		ChipHitCheck();
		// デストラクタ
		~ChipHitCheck();
		// マップチップの当たり判定登録(引数1:キー　引数2:チップ情報を格納したコンテナ)
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		// 読み込むキー情報の更新(引数:切り替え用キー)
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		// 対象に効果があるかの判定(引数:判定を行うチップ番号)
		int IsChipType(const int no);
		// 対象に当たり判定があるかの判定(引数:判定を行うチップ番号)
		bool IsHitType(const int no);
		// 対象の当たり判定を取得(引数:取得を行うチップ番号)
		AABB ChipCollision(const int no);
	private:
		// (キー:ステージキー　値:対応ステージの当たり判定)
		using StageMaps = std::unordered_map<std::string, ChipsMap>;
		std::string _chipKey;		// 取り出すマップチップ情報(キー)
		StageMaps _stageChipsMap;	// ステージの情報
		// 登録情報の初期化
		void ClearStageMaps();
	};
}

