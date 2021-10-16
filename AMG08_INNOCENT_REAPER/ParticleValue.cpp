/*****************************************************************//**
 * \file   ParticleValue.cpp
 * @brief  パーティクル画像の情報
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#include "ParticleValue.h"
#include "ImageValue.h"

namespace {
	constexpr auto DEFAULT_RATE = 1.0;		//!< デフォルトの拡縮率
}

namespace inr {
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
}
