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

		void FlagChange(bool fadeType, int interval = 0);	// �t�F�[�h�C���E�t�F�[�h�A�E�g�؂�ւ�(�����؂�ւ���̗P�\����)
		inline bool IsEnd() { return _end; }	// �����͏I�����Ă��邩�H
		inline bool PalChange() { return _addEnd; }	// ���Z���Z�͐؂�ւ�������H

	private:
		int _pal;	// �P�x

		int _interval;	// �����I����̑҂�����
		bool _isInterval;	// ���݂͑҂����Ԃ��H
		bool _end;	// �����͏I��������
		bool _type;	// ���Z�E���Z�����̂ǂ�����s����
		bool _addEnd;

		void FadeOut();
		void FadeIn();
		bool FadeEnd();

	};
}

