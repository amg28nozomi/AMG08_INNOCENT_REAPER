/*****************************************************************//**
 * \file   ChipHitCheck.h
 * \brief  チップ番号クラス
 *		   マップチップの登録情報
 * 
 *		   チップヒットチェッククラス
 *		   マップチップの当たり判定を管理する
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
	namespace mapchip {
		// マップチップに特殊効果はあるか？
		constexpr auto NONE = -1;		// チップ無し
		constexpr auto NORMAL = 0;		// 通常チップ(効果なし)
		constexpr auto THORM = 1;		// 棘(ダメージ判定あり)
		constexpr auto IVY = 2;			// 蔦(ぶら下がり可能)
		constexpr auto TRANSITION = 3;	// ステージ遷移
		// 当たり判定はあるか？
		constexpr auto HIT_ON = true;	// 当たり判定/押し出し処理有り
		constexpr auto HIT_OFF = false;	// 当たり判定/押し出し処理無し
	}
	// マップチップの当たり判定
	class ChipNumber {
	public:
		// コンストラクタ(引数1:横幅(min)　引数2:横幅(max)　引数3:縦幅(min)　引数4:縦幅(max)　引数5:マップチップの効果　引数6:当たり判定はあるか)
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		// コンストラクタ(引数1:横幅(min)　引数2:横幅(max)　引数3:マップチップの効果　引数4:当たり判定フラグ)
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		// コンストラクタ(引数1:マップチップの効果　引数2:当たり判定フラグ)
		ChipNumber(int type, bool ishit = mapchip::HIT_ON);
		// 横幅(min)の取得
		inline int WidthMin() { return widthMin; }
		// 横幅(max)の取得
		inline int WidthMax() { return widthMax; }
		// 縦幅(min)の取得
		inline int HeightMin() { return heightMin; }
		// 縦幅(max)の取得
		inline int HeightMax() { return heightMax; }
		// チップの種類の取得
		inline int ChipType() { return chipType; }
		// 当たり判定フラグの取得
		inline bool HitType() { return hitType; }
	private:
		int widthMin;	// 横幅(Min)
		int widthMax;	// 横幅(Max)
		int heightMin;	// 縦幅(Min)
		int heightMax;	// 縦幅(Max)
		int chipType;	// チップの種類
		bool hitType;	// 当たり判定はあるか
	};
	// マップチップの当たり判定を管理する
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

