#pragma once
#include "Image.h"
#include "RedGreenBlue.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {

	namespace titles {
		constexpr auto TITLE_ALI = "title_logo_in";
		constexpr auto TITLE_ALO = "title_logo_out";
		constexpr auto TITLE_T = "title_animation";
		constexpr auto TITLE_TLI = "team_logo_in";
		constexpr auto TITLE_TLO = "team_logo_out";
	}

	class Logo : public Image {
	public:
		Logo(Game& game);
		~Logo() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		int _number;	// 現在何番目の処理を行っているか？
		int _fCount;	// フレームカウンタ
		int _maxFrame;	// 最大再生フレーム
		int _allnum;	// 画像数
		int _wait;	// 待ち時間
		bool _calculation;	// 偽:フェードイン、真:フェードアウト
		bool _animation;	// アニメーションを行うか否か

		RedGreenBlue _rgb;
		std::unique_ptr<TitleLogo> _tlogo;

		void ChangeGraph();	// ロゴの切り替え及び、値の初期化(キー入力が有った場合、処理をスキップする)
		void AnimationInit();	// アニメーションの初期化
		void AnimaSkip();
	};
}

