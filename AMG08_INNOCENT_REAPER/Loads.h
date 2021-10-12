/*****************************************************************//**
 * \file   Loads.h
 * \brief  ロードクラス
 *		   各種素材の読み込み処理を行う
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Scenario.h"
#include "ImageValue.h"

namespace inr {
	class ObjectValue;	// 二重インクルード防止
	// 各種素材およびオブジェクト情報の生成を行うクラス
	class Loads {
	public:
		// 各種素材の読み込み
		static void ResourceLoad();
		// 画像生成情報の生成および取得
		static ImageMap LoadImages();
		// 前景情報の生成および取得
		static std::vector<ImageValue> LoadForeGround1();
		// チュートリアル画像の配置情報の取得(チュートリアル1)
		static std::vector<TutorialValue> LoadTutorialF();
		// チュートリアル画像の配置情報の取得(チュートリアル2)
		static std::vector<TutorialValue> LoadTutorialS();
		// オブジェクトの配置情報の生成および取得
		// チュートリアル1(封印の祭壇)
		static std::vector<ObjectValue> LoadScenarioS_1();
		// オブジェクトの配置情報の生成および取得
		// チュートリアル2(封印の祭壇)
		static std::vector<ObjectValue> LoadScenarioS();
		// オブジェクトの配置情報の生成および取得
		// ステージ1(荒れた町)
		static std::vector<ObjectValue> LoadScenario1();
		// オブジェクトの配置情報の生成および取得
		// ステージ2(契りの森-中央)
		static std::vector<ObjectValue> LoadScenario2();
		// オブジェクトの配置情報の生成および取得
		// ステージ2(契りの森-上部)
		static std::vector<ObjectValue> LoadScenario2_1();
		// オブジェクトの配置情報の生成および取得
		// ステージ2(契りの森-下部)
		static std::vector<ObjectValue> LoadScenario2_2();
		// オブジェクトの配置情報の生成および取得
		// ステージ3(契りの森-ボス)
		static std::vector<ObjectValue> LoadScenarioB();
	};
}

