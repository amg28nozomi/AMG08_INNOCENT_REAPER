/*****************************************************************//**
 * @file   TutorialValue.h
 * @brief  チュートリアル画像の情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ImageValue.h"
#include <vector>

namespace inr {
	/** チュートリアル画像情報 */
	class TutorialValue {
	public:
		/**
		 * @brief					コンストラクタ
		 */
		TutorialValue();
		/**
		 * @brief					コンストラクタ
		 * @param ivalue	画像情報
		 * @param width		幅
		 * @param height	高さ
		 */
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		/**
		 * @brief					コンストラクタ
		 * @param ivalue	画像情報
		 * @param width1	幅(min)
		 * @param width2	幅(max)
		 * @param height1	高さ(min)
		 * @param height2	高さ(max)
		 */
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);
		/**
		 * @brief					画像情報の取得
		 * @return				画像情報を返す
		 */
		inline ImageValue GetImageValue() { return _ivalue; }
		/**
		 * @brief					幅の取得
		 * @return				幅を返す
		 */
		std::vector<int> Width() { return _width; }
		/**
		 * @brief					高さの取得
		 * @return				高さを返す
		 */
		std::vector<int> Height() { return _height; }
	private:
		ImageValue _ivalue;				//!< イメージ情報
		std::vector<int> _width;	//!< 幅
		std::vector<int> _height;	//!< 高さ
	};
}

