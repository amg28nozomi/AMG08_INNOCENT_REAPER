/*****************************************************************//**
 * \file   Image.h
 * \brief  �摜�̃X�[�p�[�N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	namespace background {
		// �w�i�f�ނ̊e����
		// ����̍Ւd
		constexpr auto BACK_GROUND_S = "back_groundS";					// �摜�L�[
		constexpr auto ALTAR = "back_altar";							// �摜�L�[(�O�i)
		constexpr auto STAGES_HEIGHT = 2160;							// �c�� 
		// �r�ꂽ��
		constexpr auto BACK_GROUND_1 = "back_ground1";					// �摜�L�[
		constexpr auto STAGE1_MAP_HEIGHT = 2160;						// �c��
		// �_��̐X
		constexpr auto BACK_GROUND_2 = "back_ground2";					// �摜�L�[
		constexpr auto STAGE2_MAP_HEIGHT = 2160;						// �c��
		// �{�X�X�e�[�W
		constexpr auto BACK_GROUND_B = "back_groundB";					// �摜�L�[
	}
	namespace end {
		// �X�^�b�t���[��
		constexpr auto STAFF_ROLL = "staff_roll";						// �摜�L�[		
		constexpr auto STAFF_ROLL_WIDTH = 1920;							// ����
		constexpr auto STAFF_ROLL_HEIGHT = 11880;						// �c��
	}
	// �A�j���[�V�����f��
	// �^�C�g���w�i
	constexpr auto BACK_GROUND = "backP";								// �摜�L�[
	constexpr auto BACK_GROUND_W = 1920;								// ����
	constexpr auto BACK_GROUND_H = 2160;								// �c��
	// �^�C�g�����S
	constexpr auto TITLE_LOGO = "title_rogo";							// �摜�L�[
	constexpr auto TITLE_IMAGE_W = 1920;								// ����
	constexpr auto TITLE_IMAGE_H = 1080;								// �c��
	// �`�[�����S
	constexpr auto TEAM_LOGO = "team_logo";								// �摜�L�[
	constexpr auto TEAM_IMAGE = 140;									// �摜�T�C�Y
	// �^�C�g���w�i
	constexpr auto TITLE_BG = "title_back_ground";
	constexpr auto TITLE_IMAGE_WIDTH = 1920;
	constexpr auto TITLE_IMAGE_HEIGHT = 1080;
	// �e��^�C�g��UI�̉摜�L�[
	constexpr auto TITLE_START1 = "ui_start1";
	constexpr auto TITLE_START2 = "ui_start2";
	constexpr auto TITLE_EXIT1 = "ui_exit1";
	constexpr auto TITLE_EXIT2 = "ui_exit2";
	// �^�C�g��UI�̃T�C�Y
	constexpr auto TITLE_UI_HEIGHT = 166;
	constexpr auto TITLE_START_WIDTH = 372;
	constexpr auto TITLE_EXIT_WIDTH = 298;

	class Game;		// ��d�C���N���[�h�h�~
	// �摜�̊��N���X
	class Image {
	protected:
		Game& _game;			//�@�Q�[���Q��
		Vector2 _pos;			// ���W
		std::string _graphKey;	// �摜�L�[
	public:
		Image(Game& game);
		~Image() = default;
		// ������
		virtual void Init();
		// �X�V
		virtual void Process();
		// �`��
		virtual void Draw();
		// �t���O�̋N��
		virtual void FlagOn();
	};
}

