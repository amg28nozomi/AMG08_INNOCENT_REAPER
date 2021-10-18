/*****************************************************************//**
 * @file   ForeGround.h
 * @brief  前景クラス（画像クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "ImageValue.h"
#include <vector>

namespace inr {
	/** 前景のコンパイル時定数 */
	namespace fgd {
		// 前景の画像キー
		constexpr auto FORE_STAGE0 = "fore_stage0";				//!< ステージ0(封印の祭壇)
		constexpr auto FORE_STAGET = "fore_stageT";				//!< チュートリアルステージ(封印の祭壇)
		constexpr auto FORE_STAGE1 = "fore_stage1";				//!< ステージ1(荒れた町)
		constexpr auto FORE_STAGE2 = "fore_stage2";				//!< ステージ2(契りの森-中央)
		constexpr auto FORE_STAGE2_1 = "fore_stage2_1";		//!< ステージ2(契りの森-上部)
		constexpr auto FORE_STAGE2_2 = "fore_stage2_2";		//!< ステージ2(契りの森-下部)
		constexpr auto FORE_STAGE3 = "fore_stageb";				//!< ステージ3(契りの森-ボス)
		// ステージ0の画像サイズ
		constexpr auto FORE0_W = 3840;										//!< ステージ0(封印の祭壇)の幅
		constexpr auto FORE0_H = 2160;										//!< ステージ0(封印の祭壇)の高さ
		// チュートリアルステージの画像サイズ
		constexpr auto FORET_W = 5760;										//!< チュートリアルステージ(封印の祭壇)の幅
		constexpr auto FORET_H = 1080;										//!< チュートリアルステージ(封印の祭壇)の高さ
		// ステージ1の画像サイズ
		constexpr auto FORE1_W = 9600;										//!< ステージ1(荒れた町)の幅
		constexpr auto FORE1_H = 2160;										//!< ステージ1(荒れた町)の高さ
		// ステージ2の画像サイズ
		constexpr auto FORE2_W = 9600;										//!< ステージ2(契りの森-中央)の幅
		constexpr auto FORE2_H = 2160;										//!< ステージ2(契りの森-中央)の高さ
		// ステージ2-1の画像サイズ
		constexpr auto FORE2_1_W = 9600;									//!< ステージ2(契りの森-上部)の幅
		constexpr auto FORE2_1_H = 2160;									//!< ステージ2(契りの森-上部)の高さ
		// ステージ2-2の画像サイズ
		constexpr auto FORE2_2_W = 3840;									//!< ステージ2(契りの森-下部)の幅
		constexpr auto FORE2_2_H = 3240;									//!< ステージ2(契りの森-下部)の高さ
		// ボスステージの画像サイズ
		constexpr auto FORE3_W = 5760;										//!< ステージ3(契りの森-ボス)の幅
		constexpr auto FORE3_H = 1080;										//!< ステージ3(契りの森-ボス)の高さ
	}
	/** 前景クラス */
	class ForeGround : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ForeGround(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~ForeGround();
		/**
		 * @brief				初期化処理
		 */
		void Init() override;
		/**
		 * @brief				描画処理
		 */
		void Draw() override;
		/**
		 * @brief				前景の設定
		 * @param ive		画像情報を格納した動的配列
		 */
		void SetForeGround(std::vector<ImageValue> ive);
		/**
		 * @brief				描画情報の設定
		 * @param key		ステージキー
		 * @return			設定に成功した場合はtrueを返す
		 *							失敗した場合はfalseを返す
		 */
		bool SetKey(const std::string key);
	private:
		std::vector<ImageValue> _ivalue;	//!< 前景を管理するコンテナ
		bool _draw;												//!< 描画を行うかのフラグ
		/**
		 * @brief				添え字の取得
		 * @param key		ステージキー
		 * @return			成功した場合は添え字を返す
		 *							失敗した場合は-1を返す
		 */
		int CheckKey(const std::string key);
	};
}

