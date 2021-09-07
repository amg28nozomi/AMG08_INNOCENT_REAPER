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

		inline void FlagChange(bool fadeType) { _type = fadeType; }	// フェードイン・フェードアウト切り替え
	private:
		int _pal;	// 輝度

		int _count;	// 何フレーム生存するか
		bool _calculation;	// 加算減算の判定()
		bool _end;	// 処理は終了したか
		bool _type;	// 加算・減算処理のどちらを行うか

		void FadeOut();
		void FadeIn();
		bool FadeEnd();

	};
}

