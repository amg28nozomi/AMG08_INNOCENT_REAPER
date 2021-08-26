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
		int _number;	// ���݉��Ԗڂ̏������s���Ă��邩�H
		int _fCount;	// �t���[���J�E���^
		int _maxFrame;	// �ő�Đ��t���[��
		int _allnum;	// �摜��
		int _wait;	// �҂�����
		bool _calculation;	// �U:�t�F�[�h�C���A�^:�t�F�[�h�A�E�g
		bool _animation;	// �A�j���[�V�������s�����ۂ�

		RedGreenBlue _rgb;
		std::unique_ptr<TitleLogo> _tlogo;

		void ChangeGraph();	// ���S�̐؂�ւ��y�сA�l�̏�����(�L�[���͂��L�����ꍇ�A�������X�L�b�v����)
		void AnimationInit();	// �A�j���[�V�����̏�����
		void AnimaSkip();
	};
}

