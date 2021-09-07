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

		inline void FlagChange(bool fadeType) { _type = fadeType; }	// �t�F�[�h�C���E�t�F�[�h�A�E�g�؂�ւ�
	private:
		int _pal;	// �P�x

		int _count;	// ���t���[���������邩
		bool _calculation;	// ���Z���Z�̔���()
		bool _end;	// �����͏I��������
		bool _type;	// ���Z�E���Z�����̂ǂ�����s����

		void FadeOut();
		void FadeIn();
		bool FadeEnd();

	};
}

