#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {

	namespace image {
		constexpr auto BLACK = "black";

		constexpr auto FADE_OUT = true;
		constexpr auto FADE_IN = false;
	}

	class FadeBlack : public Image {
	public:
		FadeBlack(Game& game);
		~FadeBlack();

		void Init() override;
		void Process() override;
		void Draw() override;

		void FlagChange(bool fadeType, int interval = 0);	// フェードイン・フェードアウト切り替え(処理切り替え後の猶予時間)
		inline bool IsEnd() { return _end; }	// 処理は終了しているか？
		inline bool PalChange() { return _addEnd; }	// 加算減算は切り替わったか？

	private:
		int _pal;	// 輝度

		int _interval;	// 処理終了後の待ち時間
		bool _isInterval;	// 現在は待ち時間か？
		bool _end;	// 処理は終了したか
		bool _type;	// 加算・減算処理のどちらを行うか
		bool _addEnd;

		void FadeOut();
		void FadeIn();
		bool FadeEnd();

	};
}

