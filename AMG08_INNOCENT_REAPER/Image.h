/*****************************************************************//**
 * \file   Image.h
 * \brief  画像のスーパークラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	namespace background {
		// 背景素材の各種情報
		// 封印の祭壇
		constexpr auto BACK_GROUND_S = "back_groundS";					// 画像キー
		constexpr auto ALTAR = "back_altar";							// 画像キー(前景)
		constexpr auto STAGES_HEIGHT = 2160;							// 縦幅 
		// 荒れた町
		constexpr auto BACK_GROUND_1 = "back_ground1";					// 画像キー
		constexpr auto STAGE1_MAP_HEIGHT = 2160;						// 縦幅
		// 契りの森
		constexpr auto BACK_GROUND_2 = "back_ground2";					// 画像キー
		constexpr auto STAGE2_MAP_HEIGHT = 2160;						// 縦幅
		// ボスステージ
		constexpr auto BACK_GROUND_B = "back_groundB";					// 画像キー
	}
	namespace end {
		// スタッフロール
		constexpr auto STAFF_ROLL = "staff_roll";						// 画像キー		
		constexpr auto STAFF_ROLL_WIDTH = 1920;							// 横幅
		constexpr auto STAFF_ROLL_HEIGHT = 11880;						// 縦幅
	}
	// アニメーション素材
	// タイトル背景
	constexpr auto BACK_GROUND = "backP";								// 画像キー
	constexpr auto BACK_GROUND_W = 1920;								// 横幅
	constexpr auto BACK_GROUND_H = 2160;								// 縦幅
	// タイトルロゴ
	constexpr auto TITLE_LOGO = "title_rogo";							// 画像キー
	constexpr auto TITLE_IMAGE_W = 1920;								// 横幅
	constexpr auto TITLE_IMAGE_H = 1080;								// 縦幅
	// チームロゴ
	constexpr auto TEAM_LOGO = "team_logo";								// 画像キー
	constexpr auto TEAM_IMAGE = 140;									// 画像サイズ
	// タイトル背景
	constexpr auto TITLE_BG = "title_back_ground";
	constexpr auto TITLE_IMAGE_WIDTH = 1920;
	constexpr auto TITLE_IMAGE_HEIGHT = 1080;
	// 各種タイトルUIの画像キー
	constexpr auto TITLE_START1 = "ui_start1";
	constexpr auto TITLE_START2 = "ui_start2";
	constexpr auto TITLE_EXIT1 = "ui_exit1";
	constexpr auto TITLE_EXIT2 = "ui_exit2";
	// タイトルUIのサイズ
	constexpr auto TITLE_UI_HEIGHT = 166;
	constexpr auto TITLE_START_WIDTH = 372;
	constexpr auto TITLE_EXIT_WIDTH = 298;

	class Game;		// 二重インクルード防止
	// 画像の基底クラス
	class Image {
	protected:
		Game& _game;			//　ゲーム参照
		Vector2 _pos;			// 座標
		std::string _graphKey;	// 画像キー
	public:
		Image(Game& game);
		~Image() = default;
		// 初期化
		virtual void Init();
		// 更新
		virtual void Process();
		// 描画
		virtual void Draw();
		// フラグの起動
		virtual void FlagOn();
	};
}

