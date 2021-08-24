#pragma once
#include "Image.h"
#include "RedGreenBlue.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {

	class Logo : public Image {
	public:
		Logo(Game& game);
		~Logo() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		int _number;	// ���݉��Ԗڂ̏������s���Ă��邩�H
		int _fCount;	// �t���[���J�E���^
		int _wait;	// �҂�����
		bool _calculation;	// ���Z�����Z��

		RedGreenBlue _rgb;
		std::unique_ptr<TitleLogo> _tlogo;

		void ChangeGraph();	// ���S�̐؂�ւ��y�сA�l�̏�����(�L�[���͂��L�����ꍇ�A�������X�L�b�v����)
		void AnimationInit();	// �A�j���[�V�����̏�����

	};
}

