#include "ChipHitCheck.h"
#include "Collision.h"
#include "Vector2.h"

namespace inr {
	ChipNumber::ChipNumber(int width1, int width2, int height1, int height2, int type, bool ishit) {
		widthMin = width1;
		widthMax = width2;
		heightMin = height1;
		heightMax = height2;
		chipType = type;
		hitType = ishit;
	}

	ChipNumber::ChipNumber(int width1, int width2, int type, bool ishit) {
		widthMin = width1;
		widthMax = width2;
		heightMin = 0;
		heightMax = 40;
		chipType = type;
		hitType = ishit;
	}

	ChipHitCheck::ChipHitCheck() {
		_chipKey = "";
		ClearStageMaps();
	}

	ChipHitCheck::~ChipHitCheck() {
		ClearStageMaps();
	}

	void ChipHitCheck::ClearStageMaps() {
		for (auto&& scm : _stageChipsMap) {
			auto& key = scm.first;	// 参照元のキーで検索
			auto chip = _stageChipsMap.find(key);
			chip->second.clear();	// 連想配列初期化
		}
		_stageChipsMap.clear();
	}

	void ChipHitCheck::LoadChipsMap(std::string key, ChipsMap& chipsMap) {
		// この情報は既に登録されているか？
		auto it = _stageChipsMap.find(key);
		// ヒットした場合は処理から抜ける
		if (it != _stageChipsMap.end()) return;
		_stageChipsMap.emplace(key, chipsMap);	// 連想配列に登録
	}

	AABB ChipHitCheck::ChipCollision(const int no) {
		Vector2 min;
		Vector2 max;

		auto stage = _stageChipsMap.find(_chipKey);	// 現在のステージの連想配列を取り出す
		auto chipdata = stage->second.find(no);	// チップ番号の当たり判定を取得
		if (chipdata == stage->second.end()) {
			// 登録されていない＝判定が変わっていない
			// 40*40の通常の値を戻り値として返す
			min = { 0, 0 };
			max = { 40, 40 };
		} else {
			auto chip = chipdata->second;	// 登録されているChipNumberを取得

			min = { static_cast<double>(chip.WidthMin()), static_cast<double>(chip.HeightMin()) };
			max = { static_cast<double>(chip.WidthMax()), static_cast<double>(chip.HeightMax()) };
		}
		AABB chipbox = { min, max };
		return chipbox;
	}
}
