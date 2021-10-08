/*****************************************************************//**
 * \file   Image.cpp
 * \brief  画像のスーパークラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#include "Image.h"
#include "Vector2.h"

namespace inr {

	Image::Image(Game& game) : _game(game) {
		_graphKey = "";
		_pos = { 960, 540 };
	}
	// 初期化
	void Image::Init() {
	}
	// 更新
	void Image::Process() {
	}
	// 描画
	void Image::Draw() {
	}
	// フラグの起動
	void Image::FlagOn() {
	}
}
