#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {

	// マップの当たり判定を管理
	class ChipNumber {
	public:
		// 当たり判定に修正がない場合は引数指定なしでOK
		ChipNumber(int width1, int width2, int height1, int height2);
		ChipNumber(int width1, int width2);

		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }

	private:
		int widthMin;	// 横幅(Min)
		int widthMax;	// 横幅(Max)
		int heightMin;	// 縦幅(Min)
		int heightMax;	// 縦幅(Max)
	};

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void ClearStageMaps();
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// マップチップの当たり判定登録
		inline void ChangeStageKey(std::string nextkey) { _stageKey = nextkey; }	// 読み込むキー情報の更新
		AABB ChipCollision(const int no);	// 対象のマップチップ
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _stageKey;	// 現在のステージ
		StageMaps _stageChipsMap;	// ステージの情報
	};
}

