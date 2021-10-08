/*****************************************************************//**
 * \file   ImageValue.h
 * \brief  イメージバリュークラス
 *         パーティクルバリュークラス
 *		   チュートリアルバリュークラス
 *		   画像クラスを継承したサブクラスの各種データ
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include <vector>

namespace inr {

	namespace pvalue {
		// アイテム番号
		constexpr auto ITEM_0 = 0;
		constexpr auto ITEM_1 = 1;
		constexpr auto ITEM_2 = 2;
		constexpr auto ITEM_3 = 3;
	}

	namespace item {
		// テキスト番号
		constexpr auto MESSAGE_0 = "message_0";
		constexpr auto MESSAGE_1 = "message_1";
		constexpr auto MESSAGE_2 = "message_2";
		constexpr auto MESSAGE_3 = "message_3";
	}
	// パーティクルイメージクラスのデータ
	class ParticleValue {
	public:
		ParticleValue();
		// 引数1:アニメーション処理を行うか　引数2:透過処理を行うか　引数3:拡縮率
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		~ParticleValue() = default;
		// 各種情報の取得
		inline bool Animation() { return _animation; }
		inline bool Particle() { return _particle; }
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	// アニメーションの再生を行うか
		bool _particle;		// 透過処理を行うか
		double _exrate;		// 拡大率（デフォルトだと1.0倍）
	};
	// 画像クラスのデータ
	class ImageValue {
	public:
		ImageValue();
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		~ImageValue() = default;

		inline std::string GraphKey() { return _graph; }
		inline Vector2 Position() { return _position; }
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;	// 画像キー
		Vector2 _position;	// 座標
		ParticleValue _particleValue;
	};

	class TutorialValue {
	public:
		TutorialValue();
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);

		inline ImageValue GetImageValue() { return _ivalue; }
		std::vector<int> Width() { return _width; }
		std::vector<int> Height() { return _height; }

	private:
		ImageValue _ivalue;
		std::vector<int> _width;
		std::vector<int> _height;
	};
}

