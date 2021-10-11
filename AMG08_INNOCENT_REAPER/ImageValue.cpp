/*****************************************************************//**
 * \file   ImageValue.cpp
 * \brief  イメージバリュークラス
 *         パーティクルバリュークラス
 *		   チュートリアルバリュークラス
 *		   画像クラスを継承したサブクラスの各種データ
 * 
 * \author nozom
 * \date   October 2021
 *********************************************************************/
#include "ImageValue.h"

namespace {
	constexpr auto DEFAULT_RATE = 1.0;		// 拡縮率
}

namespace inr {
	// コンストラクタ
	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";	// キーの初期化
	}
	// コンストラクタ
	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// キーの初期化
	}
	// コンストラクタ
	ParticleValue::ParticleValue() {
		// 初期化
		_animation = false;
		_particle = false;
		_exrate = DEFAULT_RATE;
	}
	// コンストラクタ
	ParticleValue::ParticleValue(bool animation, bool particle, double exrate) {
		// 初期化
		_animation = animation;
		_particle = particle;
		_exrate = exrate;
	}
	// コンストラクタ
	TutorialValue::TutorialValue() : _ivalue() {
		// コンテナの初期化
		_width.clear();
		_height.clear();
	}
	// コンストラクタ
	TutorialValue::TutorialValue(ImageValue ivalue, int width, int height) : _ivalue(ivalue) {
		// 初期化
		_width.emplace_back(width);
		_height.emplace_back(height);
	}
	// コンストラクタ
	TutorialValue::TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2) : _ivalue(ivalue) {
		// 初期化
		_width = { width1, width2 };
		_height = { height1, height2 };
	}
}