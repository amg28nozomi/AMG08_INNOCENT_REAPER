/*****************************************************************//**
 * @file   ParticleValue.h
 * @brief  パーティクル画像の情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
	/** パーティクルイメージのコンパイル時定数 */
	namespace pvalue {
		// アイテム番号
		constexpr auto ITEM_0 = 0;								//!< アイテム1
		constexpr auto ITEM_1 = 1;								//!< アイテム2
		constexpr auto ITEM_2 = 2;								//!< アイテム3
		constexpr auto ITEM_3 = 3;								//!< アイテム4
	}
	namespace item {
		// テキスト番号
		constexpr auto MESSAGE_0 = "message_0";		//!< テキスト1
		constexpr auto MESSAGE_1 = "message_1";		//!< テキスト2
		constexpr auto MESSAGE_2 = "message_2";		//!< テキスト3
		constexpr auto MESSAGE_3 = "message_3";		//!< テキスト4
	}
	/** パーティクルイメージクラスのデータ */
	class ParticleValue {
	public:
		/**
		 * @brief						コンストラクタ
		 */
		ParticleValue();
		/**
		 * @brief						コンストラクタ
		 * @param animation	アニメーション処理を行うか
		 * @param particle	透過処理を行うか
		 * @param exrate		拡縮率
		 */
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		/**
		 * @brief	デストラクタ
		 */
		~ParticleValue() = default;
		// アニメーションフラグの取得
		/**
		 * @brief		アニメーションフラグの取得
		 * @return	アニメーションフラグを返す
		 */
		inline bool Animation() { return _animation; }
		/**
		 * @brief		透過処理フラグの取得
		 * @return	透過処理フラグを返す
		 */
		inline bool Particle() { return _particle; }
		/**
		 * @brief		拡大率の取得
		 * @return	拡大率を返す
		 */
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	//!< アニメーションの再生を行うか
		bool _particle;		//!< 透過処理を行うか
		double _exrate;		//!< 拡大率（デフォルトだと1.0倍）
	};
}

