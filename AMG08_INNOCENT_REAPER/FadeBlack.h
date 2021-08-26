#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {

	namespace image {
		constexpr auto BLACK = "black";

		constexpr auto FADEIN = true;
		constexpr auto FADEOUT = false;
	}

	class FadeBlack : public Image {
	public:
		FadeBlack(Game& game);
		~FadeBlack();

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		RedGreenBlue _rgb;	// 色調

		int _count;	// 何フレーム生存するか
		bool _calculation;	// 加算減算の判定()
		bool _end;	// 処理は終了したか

	};
}

