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
	}

	// マップの当たり判定を管理
	class ChipNumber {
	public:
		// 当たり判定に修正がない場合は引数指定なしでOK
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL);
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL);

		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }
		inline int ChipType() { return chipType; }

	private:
		int widthMin;	// 横幅(Min)
		int widthMax;	// 横幅(Max)
		int heightMin;	// 縦幅(Min)
		int heightMax;	// 縦幅(Max)
		int chipType;	// チップの種類
	};

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// マップチップの当たり判定登録
		inline void ChangeStageKey(std::string nextkey) { _stageKey = nextkey; }	// 読み込むキー情報の更新
		AABB ChipCollision(const int no);	// 対象のマップチップ
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _stageKey;	// 現在のステージ
		StageMaps _stageChipsMap;	// ステージの情報

		void ClearStageMaps();
	};
}

