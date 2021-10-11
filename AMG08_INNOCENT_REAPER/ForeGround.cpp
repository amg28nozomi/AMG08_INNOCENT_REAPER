/*****************************************************************//**
 * \file   ForeGround.cpp
 * \brief  前景クラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#include "ForeGround.h"
#include "ImageValue.h"
#include "Game.h"
#include "StageTransition.h"
#include "Loads.h"
#include <DxLib.h>

namespace inr {
	// コンストラクタ
	ForeGround::ForeGround(Game& game) : Image(game) {
		// コンテナの取得
		_ivalue = Loads::LoadForeGround1();
		_draw = false;
	}
	// デストラクタ
	ForeGround::~ForeGround() {
		_ivalue.clear();	// コンテナの解放
	}
	// 初期化
	void ForeGround::Init() {
	}
	// 前景の設定
	void ForeGround::SetForeGround(std::vector<ImageValue> ive) {
		_ivalue = ive;	// コンテナの登録
		return;
	}
	// 描画
	void ForeGround::Draw() {
		// 描画座標の算出
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_graphKey, 0);	// 画像の取得
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);				// 描画
	}
	// 描画情報の設定
	bool ForeGround::SetKey(const std::string key) {
		auto ckey = CheckKey(key);		// キーの読み込み
		if (ckey == -1) return false;	// 読み込み失敗
		switch (ckey) {
		case 0:	// ステージ0
			_draw = true;							// 追加描画あり
			_graphKey = _ivalue[ckey].GraphKey();	// 画像
			_pos = _ivalue[ckey].Position();		// 座標
			return true;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			_draw = false;							// 追加描画なし
			_graphKey = _ivalue[ckey].GraphKey();	// 画像
			_pos = _ivalue[ckey].Position();		// 座標
			return true;
		default:
			return false;	// 該当なし
		}
	}
	// 添え字の算出
	int ForeGround::CheckKey(const std::string key) {
		// キーに対応した添え字を返す
		if (key == stage::STAGE_0) return 0;
		else if (key == stage::STAGE_T) return 1;
		else if (key == stage::STAGE_1) return 2;
		else if (key == stage::STAGE_2) return 3;
		else if (key == stage::STAGE_2_1) return 4;
		else if (key == stage::STAGE_2_2) return 5;
		else if (key == stage::STAGE_3) return 6;
		else return -1;		// 該当なし
	}
}