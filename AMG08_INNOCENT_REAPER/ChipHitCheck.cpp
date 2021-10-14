/*****************************************************************//**
 * @file   ChipHitCheck.cpp
 * @brief  マップチップの登録情報を管理するチップヒットチェッククラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ChipHitCheck.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipNumber.h"

namespace inr {
	/** コンストラクタ */
	// コンストラクタ
	ChipHitCheck::ChipHitCheck() {
		// 各種初期化
		_chipKey = "";
		ClearStageMaps();
	}
	// デストラクタ
	ChipHitCheck::~ChipHitCheck() {
		ClearStageMaps();
	}
	// 登録情報の初期化
	void ChipHitCheck::ClearStageMaps() {
		for (auto&& scm : _stageChipsMap) {
			auto& key = scm.first;	// 参照元のキーで検索
			auto chip = _stageChipsMap.find(key);
			chip->second.clear();	// 連想配列初期化
		}
		_stageChipsMap.clear();		// コンテナの解放
	}
	// マップチップの当たり判定登録
	void ChipHitCheck::LoadChipsMap(std::string key, ChipsMap& chipsMap) {
		// この情報は既に登録されているか？
		auto it = _stageChipsMap.find(key);
		// ヒットした場合は処理から抜ける
		if (it != _stageChipsMap.end()) return;
		_stageChipsMap.emplace(key, chipsMap);	// 連想配列に登録
	}
	// 対象に効果があるかの判定(引数:判定を行うチップ番号)
	int ChipHitCheck::IsChipType(const int no) {
		auto stage = _stageChipsMap.find(_chipKey);	// 現在のステージの連想配列を取り出す
		auto chipnumber = stage->second.find(no);	// チップ番号の当たり判定を取得
		if (chipnumber == stage->second.end()) return mapchip::NORMAL;	// 該当なし
		// 効果はあるか？
		switch (chipnumber->second.ChipType()) {
		case mapchip::NORMAL:
			return mapchip::NORMAL;		// 効果なし
		case mapchip::THORM:
			return mapchip::THORM;		// ダメージ判定
		case mapchip::IVY:
			return mapchip::IVY;		// 蔦判定
		case mapchip::TRANSITION:
			return mapchip::TRANSITION;	// ステージ遷移判定
		default:
			return mapchip::NONE;		// 登録されていないチップ番号
		}
	}
	// 対象に当たり判定があるかの判定(引数:判定を行うチップ番号)
	bool ChipHitCheck::IsHitType(const int no) {
		auto stage = _stageChipsMap.find(_chipKey);	// 現在のステージの連想配列を取り出す
		auto chipnumber = stage->second.find(no);	// チップ番号の当たり判定を取得
		if (chipnumber == stage->second.end()) return true;
		// 当たり判定はあるか？
		switch (chipnumber->second.HitType()) {
		case mapchip::HIT_ON:	// 当たり判定有り
			return true;
		case mapchip::HIT_OFF:	// 当たり判定無し
			return false;
		}
	}
	// 対象の当たり判定を取得(引数:取得を行うチップ番号)
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
			auto chip = chipdata->second;	// 登録されているChipNumberを取
			min = { static_cast<double>(chip.WidthMin()), static_cast<double>(chip.HeightMin()) };
			max = { static_cast<double>(chip.WidthMax()), static_cast<double>(chip.HeightMax()) };
		}
		AABB chipbox = { min, max };
		return chipbox;
	}
}
