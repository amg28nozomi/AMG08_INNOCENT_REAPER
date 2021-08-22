#pragma once
#include "Image.h"

namespace inr {

	class Logo : public Image {
	public:
		Logo(Game& game);
		~Logo() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		int _pal;	// �P�x
		int _number;	// ���݉��Ԗڂ̏������s���Ă��邩�H

		void ChangeGraph();	// ���S�̐؂�ւ��y�сA�l�̏�����(�L�[���͂��L�����ꍇ�A�������X�L�b�v����)
		void AnimationInit();	// �A�j���[�V�����̏�����

	};
}

