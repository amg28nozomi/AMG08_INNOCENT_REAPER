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
		// コンストラクタ
		ParticleValue();
		// コンストラクタ(引数1:アニメーション処理を行うか　引数2:透過処理を行うか　引数3:拡縮率)
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		/**
		 * @brief	デストラクタ
		 */
		~ParticleValue() = default;
		// アニメーションフラグの取得
		inline bool Animation() { return _animation; }
		// 透過処理を行うかの取得
		inline bool Particle() { return _particle; }
		// 拡大率の取得
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	// アニメーションの再生を行うか
		bool _particle;		// 透過処理を行うか
		double _exrate;		// 拡大率（デフォルトだと1.0倍）
	};
	// 画像クラスのデータ
	class ImageValue {
	public:
		// コンストラクタ
		ImageValue();
		// コンストラクタ(引数1:画像キー　引数2:描画座標　引数3:パーティクルイメージの情報)
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		/**
		 * @brief	デストラクタ
		 */
		~ImageValue() = default;
		// 画像キーの取得
		inline std::string GraphKey() { return _graph; }
		// 座標の取得
		inline Vector2 Position() { return _position; }
		// パーティクルイメージ情報の取得
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;				// 画像キー
		Vector2 _position;				// 座標
		ParticleValue _particleValue;	// パーティクルイメージの情報
	};
	// チュートリアル画像クラスのデータ
	class TutorialValue {
	public:
		// コンストラクタ
		TutorialValue();
		// コンストラクタ(引数1:画像情報　引数2:横幅　引数3:縦幅)
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		// コンストラクタ(引数1:画像情報　引数2:横幅(min)　引数3:横幅(max)　引数4:縦幅(min)　引数5:縦幅(max))
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);
		// 画像情報の取得
		inline ImageValue GetImageValue() { return _ivalue; }
		// 横幅の取得
		std::vector<int> Width() { return _width; }
		// 縦幅の取得
		std::vector<int> Height() { return _height; }
	private:
		ImageValue _ivalue;			// イメージ情報
		std::vector<int> _width;	// 横幅
		std::vector<int> _height;	// 縦幅
	};
}

