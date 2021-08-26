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
		RedGreenBlue _rgb;	// �F��

		int _count;	// ���t���[���������邩
		bool _calculation;	// ���Z���Z�̔���()
		bool _end;	// �����͏I��������

	};
}

