#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {

	namespace mapchip {
		// マップチップに特殊効果はあるか？
		constexpr auto NORMAL = 0;	// 通常チップ(効果なし)
		constexpr auto THORM = 1;	// 棘(ダメージ判定あり)
		constexpr auto IVY = 2;		// 蔦(ぶら下がり可能)

		constexpr auto HIT_ON = true;	// 当たり判定/押し出し処理有り
		constexpr auto HIT_OFF = false;	// 当たり判定/押し出し処理無し
	}

	// マップの当たり判定を管理
	class ChipNumber {
	public:
		// 当たり判定に修正がない場合は引数指定なしでOK
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);

		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }
		inline int ChipType() { return chipType; }
		inline bool HitType() { return hitType; }

	private:
		int widthMin;	// 横幅(Min)
		int widthMax;	// 横幅(Max)
		int heightMin;	// 縦幅(Min)
		int heightMax;	// 縦幅(Max)
		int chipType;	// チップの種類
		bool hitType;	// 当たり判定はあるか
	};

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// マップチップの当たり判定登録
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }	// 読み込むキー情報の更新
		int IsChipType(ChipNumber chipnumber);
		bool IsHitType(ChipNumber chipnumber);	// 対象に当たり判定はあるか？
		AABB ChipCollision(const int no);	// 対象のマップチップ
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _chipKey;	// 取り出すマップチップ情報(キー)
		StageMaps _stageChipsMap;	// ステージの情報

		void ClearStageMaps();
	};
}

