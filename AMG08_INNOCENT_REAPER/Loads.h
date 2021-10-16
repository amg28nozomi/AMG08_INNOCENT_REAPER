/*****************************************************************//**
 * @file   Loads.h
 * @brief  各種素材の読み込み処理を行うロードクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Scenario.h"
#include "ImageValue.h"

namespace inr {
	/** 二重インクルード防止 */
	class ObjectValue;
	/** 各種素材およびオブジェクト情報の生成を行うロードクラス */
	class Loads {
	public:
		/**
		 * @brief	各種素材の読み込み
		 */
		static void ResourceLoad();
		/**
		 * @brief		画像情報の生成および取得
		 * @return	画像情報を格納した連想配列を返す
		 */
		static ImageMap LoadImages();
		/**
		 * @brief		前景情報の生成および取得
		 * @return	前景情報を格納したコンテナを返す
		 */
		static std::vector<ImageValue> LoadForeGround1();
		/**
		 * @brief		チュートリアル画像の配置情報の取得(チュートリアル1)
		 * @return	チュートリアル画像情報を格納したコンテナを返す
		 */
		static std::vector<TutorialValue> LoadTutorialF();
		/**
		 * @brief		チュートリアル画像の配置情報の取得(チュートリアル2)
		 * @return	チュートリアル画像情報を格納したコンテナを返す
		 */
		static std::vector<TutorialValue> LoadTutorialS();
		/**
		 * @brief		チュートリアルステージ(封印の祭壇)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenarioS_1();
		/**
		 * @brief		ステージ0(封印の祭壇)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenarioS();
		/**
		 * @brief		ステージ1(荒れた町)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenario1();
		/**
		 * @brief		ステージ2(契りの森-中央)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenario2();
		/**
		 * @brief		ステージ2(契りの森-上部)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenario2_1();
		/**
		 * @brief		ステージ2(契りの森-下部)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenario2_2();
		/**
		 * @brief		ステージ3(契りの森-ボス)のオブジェクト配置情報の生成および取得
		 * @return	オブジェクト情報を格納したコンテナを返す
		 */
		static std::vector<ObjectValue> LoadScenarioB();
	};
}

