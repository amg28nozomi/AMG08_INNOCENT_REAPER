/*****************************************************************//**
 * \file   ForeGround.h
 * \brief  前景クラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "ImageValue.h"
#include <vector>

namespace inr {
	namespace fgd {
		// 前景の画像キー
		constexpr auto FORE_STAGE0 = "fore_stage0";			// ステージ0(封印の祭壇)
		constexpr auto FORE_STAGET = "fore_stageT";			// チュートリアルステージ(封印の祭壇)
		constexpr auto FORE_STAGE1 = "fore_stage1";			// ステージ1(荒れた町)
		constexpr auto FORE_STAGE2 = "fore_stage2";			// ステージ2(契りの森-中央)
		constexpr auto FORE_STAGE2_1 = "fore_stage2_1";		// ステージ2(契りの森-上部)
		constexpr auto FORE_STAGE2_2 = "fore_stage2_2";		// ステージ2(契りの森-下部)
		constexpr auto FORE_STAGE3 = "fore_stageb";			// ステージ3(契りの森-ボス)
		// ステージ0の画像サイズ
		constexpr auto FORE0_W = 3840;
		constexpr auto FORE0_H = 2160;
		// チュートリアルステージの画像サイズ
		constexpr auto FORET_W = 5760;
		constexpr auto FORET_H = 1080;
		// ステージ1の画像サイズ
		constexpr auto FORE1_W = 9600;
		constexpr auto FORE1_H = 2160;
		// ステージ2の画像サイズ
		constexpr auto FORE2_W = 9600;
		constexpr auto FORE2_H = 2160;
		// ステージ2-1の画像サイズ
		constexpr auto FORE2_1_W = 9600;
		constexpr auto FORE2_1_H = 2160;
		// ステージ2-2の画像サイズ
		constexpr auto FORE2_2_W = 3840;
		constexpr auto FORE2_2_H = 3240;
		//ボスステージの画像サイズ
		constexpr auto FORE3_W = 5760;
		constexpr auto FORE3_H = 1080;
	}
	// 前景クラス
	class ForeGround : public Image {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		ForeGround(Game& game);
		// デストラクタ
		~ForeGround();
		// 初期化
		void Init() override;
		// 描画
		void Draw() override;
		// 前景の設定(引数:画像情報の動的配列)
		void SetForeGround(std::vector<ImageValue> ive);
		// 描画情報の設定(引数:ステージのキー情報)
		bool SetKey(const std::string key);
	private:
		std::vector<ImageValue> _ivalue;	// 前景を管理するコンテナ
		bool _draw;							// 描画を行うかのフラグ
		// 添え字の算出(引数:ステージのキー情報)
		int CheckKey(const std::string key);
	};
}

