/*****************************************************************//**
 * @file   ImageValue.h
 * @brief  イメージ画像の情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include "ParticleValue.h"
#include <string>
#include <vector>

namespace inr {
	/** 画像情報 */
	class ImageValue {
	public:
		/**
		 * @brief								コンストラクタ
		 */
		ImageValue();
		/**
		 * @brief								コンストラクタ
		 * @param gkey					画像キー
		 * @param pos						生成座標
		 * @param paricleValue	パーティクル画像の情報
		 */
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		/**
		 * @brief								デストラクタ
		 */
		~ImageValue() = default;
		/**
		 * @brief								画像キーの取得
		 * @return							画像キーの返す
		 */
		inline std::string GraphKey() { return _graph; }
		/**
		 * @brief								生成座標の取得
		 * @return							生成座標を返す
		 */
		inline Vector2 Position() { return _position; }
		/**
		 * @brief								パーティクル画像情報の取得
		 * @return							パーティクル画像情報を返す
		 */
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;						//!< 画像キー
		Vector2 _position;						//!< 座標
		ParticleValue _particleValue;	//!< パーティクルイメージの情報
	};
}

