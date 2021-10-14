/*****************************************************************//**
 * \file   FadeBlack.h
 * \brief  �t�F�[�h�u���b�N�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {
	namespace image {
		// �摜�L�[
		constexpr auto BLACK = "black";
		// �Ó]�t���O
		constexpr auto FADE_OUT = true;
		constexpr auto FADE_IN = false;
	}
	// �t�F�[�h�u���b�N�N���X
	class FadeBlack : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		FadeBlack(Game& game);
		// �f�X�g���N�^
		~FadeBlack();
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
		// �t�F�[�h�C���E�t�F�[�h�A�E�g�؂�ւ�����
		// ����1:�t�F�[�h�C���E�t�F�[�h�A�E�g�̂ǂ�����s����
		// true:�t�F�[�h�A�E�g�@false:�t�F�[�h�C��
		// ����2:�������J�n����܂ł̗P�\����
		void FlagChange(bool fadeType, int interval = 0);
		// �����͏I�����Ă��邩�̎擾
		inline bool IsEnd() { return _end; }
		// ���Z�E���Z�������I�����Ă��邩�̎擾
		inline bool PalChange() { return _addEnd; }
	private:
		int _pal;			// �P�x
		int _interval;		// �����I����̑҂�����
		bool _isInterval;	// ���݂͑҂����Ԃ��H
		bool _end;			// �����͏I��������
		bool _type;			// ���Z�E���Z�����̂ǂ�����s����
		bool _addEnd;		// ���Z�E���Z�������I�����Ă��邩
		// �t�F�[�h�A�E�g����
		void FadeOut();
		// �t�F�[�h�C������
		void FadeIn();
		// �������I�����Ă邩�̔��菈��
		bool FadeEnd();
	};
}

