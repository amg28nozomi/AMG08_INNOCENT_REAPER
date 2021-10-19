/*****************************************************************//**
 * @file   ImageValue.cpp
 * @brief  イメージ画像の情報
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace inr {

	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";		// キーの初期化
	}

	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// キーの初期化
	}
}