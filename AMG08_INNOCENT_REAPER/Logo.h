#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {

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
		int _wait;	// 待ち時間
		double _pal;	// 輝度
		bool _calculation;	// 加算か減算か

		RedGreenBlue _rgb;

		void ChangeGraph();	// ロゴの切り替え及び、値の初期化(キー入力が有った場合、処理をスキップする)
		void AnimationInit();	// アニメーションの初期化

	};
}

