/*****************************************************************//**
 * \file   ForeGround.h
 * \brief  �O�i�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "ImageValue.h"
#include <vector>

namespace inr {
	namespace fgd {
		// �O�i�̉摜�L�[
		constexpr auto FORE_STAGE0 = "fore_stage0";			// �X�e�[�W0(����̍Ւd)
		constexpr auto FORE_STAGET = "fore_stageT";			// �`���[�g���A���X�e�[�W(����̍Ւd)
		constexpr auto FORE_STAGE1 = "fore_stage1";			// �X�e�[�W1(�r�ꂽ��)
		constexpr auto FORE_STAGE2 = "fore_stage2";			// �X�e�[�W2(�_��̐X-����)
		constexpr auto FORE_STAGE2_1 = "fore_stage2_1";		// �X�e�[�W2(�_��̐X-�㕔)
		constexpr auto FORE_STAGE2_2 = "fore_stage2_2";		// �X�e�[�W2(�_��̐X-����)
		constexpr auto FORE_STAGE3 = "fore_stageb";			// �X�e�[�W3(�_��̐X-�{�X)
		// �X�e�[�W0�̉摜�T�C�Y
		constexpr auto FORE0_W = 3840;
		constexpr auto FORE0_H = 2160;
		// �`���[�g���A���X�e�[�W�̉摜�T�C�Y
		constexpr auto FORET_W = 5760;
		constexpr auto FORET_H = 1080;
		// �X�e�[�W1�̉摜�T�C�Y
		constexpr auto FORE1_W = 9600;
		constexpr auto FORE1_H = 2160;
		// �X�e�[�W2�̉摜�T�C�Y
		constexpr auto FORE2_W = 9600;
		constexpr auto FORE2_H = 2160;
		// �X�e�[�W2-1�̉摜�T�C�Y
		constexpr auto FORE2_1_W = 9600;
		constexpr auto FORE2_1_H = 2160;
		// �X�e�[�W2-2�̉摜�T�C�Y
		constexpr auto FORE2_2_W = 3840;
		constexpr auto FORE2_2_H = 3240;
		//�{�X�X�e�[�W�̉摜�T�C�Y
		constexpr auto FORE3_W = 5760;
		constexpr auto FORE3_H = 1080;
	}
	// �O�i�N���X
	class ForeGround : public Image {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		ForeGround(Game& game);
		// �f�X�g���N�^
		~ForeGround();
		// ������
		void Init() override;
		// �`��
		void Draw() override;
		// �O�i�̐ݒ�(����:�摜���̓��I�z��)
		void SetForeGround(std::vector<ImageValue> ive);
		// �`����̐ݒ�(����:�X�e�[�W�̃L�[���)
		bool SetKey(const std::string key);
	private:
		std::vector<ImageValue> _ivalue;	// �O�i���Ǘ�����R���e�i
		bool _draw;							// �`����s�����̃t���O
		// �Y�����̎Z�o(����:�X�e�[�W�̃L�[���)
		int CheckKey(const std::string key);
	};
}

