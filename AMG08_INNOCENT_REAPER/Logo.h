/*****************************************************************//**
 * \file   Logo.h
 * \brief  ロゴクラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {
	namespace titles {
		// タイトルアニメーションの画像キー
		constexpr auto TITLE_ALI = "title_logo_in";		// AMGロゴフェードイン
		constexpr auto TITLE_ALO = "title_logo_out";	// AMGロゴフェードアウト
		constexpr auto TITLE_T = "title_animation";		// タイトル画面アニメーション
		constexpr auto TITLE_TLI = "team_logo_in";		// チームロゴフェードイン
		constexpr auto TITLE_TLO = "team_logo_out";		// チームロゴフェードアウト
	}
	// ロゴクラス
	class Logo : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Logo(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Logo() = default;
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
	private:
		int _number;						// 現在何番目の処理を行っているか？
		int _fCount;						// フレームカウンタ
		int _maxFrame;						// 最大再生フレーム
		int _allnum;						// 画像数
		int _wait;							// 待ち時間
		bool _calculation;					// 偽:フェードイン、真:フェードアウト
		bool _animation;					// アニメーションを行うか否か
		RedGreenBlue _rgb;					// RGB
		std::unique_ptr<TitleLogo> _tlogo;	// タイトルロゴ
		// ロゴ画像の切り替え
		void ChangeGraph();
		// アニメーションの初期化
		void AnimationInit();
		// アニメーションのスキップ
		void AnimaSkip();
	};
}

