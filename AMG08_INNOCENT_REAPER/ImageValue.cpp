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

namespace inr {
	// コンストラクタ
	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";	// キーの初期化
	}
	// コンストラクタ
	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;	// キーの初期化
	}
}