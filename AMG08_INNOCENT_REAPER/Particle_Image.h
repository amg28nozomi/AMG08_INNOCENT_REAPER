/*****************************************************************//**
 * @file   Particle_Image.h
 * @brief  �p�[�e�B�N���摜�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"

namespace inr {
	/** �摜�̃R���p�C�����萔 */
	namespace image {
		/** �p�[�e�B�N���摜�̃R���p�C�����萔 */
		namespace particle {
			constexpr auto BG_BLACK = "bg_black1";					//!< ���w�i
			constexpr auto BG_OPERATION = "oparation";			//!< �������
			constexpr auto BG_SIZE_W = 1920;								//!< �T�C�Y(��)
			constexpr auto BG_SIZE_H = 1080;								//!< �T�C�Y(����)
			// �|�[�YUI
			constexpr auto CONTINUE = "continue";						//!< �R���e�B�j���[
			constexpr auto CONTROLS = "controls";						//!< �R���g���[��
			constexpr auto QUIT_TO_TITLE = "quit_to_title";	//!< �Q�[���I��
			// �T�C�Y
			constexpr auto CONTINUE_W = 272;								//!< �R���e�B�j���[�T�C�Y(��)
			constexpr auto CONTROLS_W = 284;								//!< �R���g���[���T�C�Y(��)
			constexpr auto QUIT_TO_TITLE_W = 365;						//!< �Q�[���I��(��)
			constexpr auto UI_H_1 = 55;											//!< UI����(1)
			constexpr auto UI_H_2 = 64;											//!< UI����(2)
			// UI
			constexpr auto CURSOR = "cursor";								//!< �J�[�\��
			constexpr auto CURSOR_W = 500;									//!< �J�[�\���̕�
			constexpr auto CURSOR_H = 150;									//!< �J�[�\���̍���
			constexpr auto STAGE_UI = "ui_stage";						//!< �X�e�[�WUI
			constexpr auto STAGE_UI_W = 692;								//!< �X�e�[�WUI�̕�
			constexpr auto STAGE_UI_H = 279;								//!< �X�e�[�WUI�̍���
			constexpr auto NORMAL = false;									//!< �C���[�W�^�C�v(�ʏ�)
			constexpr auto UI = true;												//!< �C���[�W�^�C�v(UI)
		}
	}
	/** �A�j���[�V�����̃R���p�C�����萔 */
	namespace animation {
		constexpr auto A_SUB = -1;												//!< ���Z
		constexpr auto A_NORMAL = 0;											//!< �ʏ�
		constexpr auto A_ADD = 1;													//!< ���Z
	}
	/** ��d�C���N���[�h�h�~ */
	class ImageValue;
	/** �p�[�e�B�N���摜 */
	class Particle_Image : public Image {
	protected:
		int _animation;			//!< �A�j���[�V�������s����
		int _pal;						//!< �����l
		double _maxExt;			//!< �ő�g�嗦
		double _extRate;		//!< �g�嗦
		bool _isDraw;				//!< �`�揈�����s���Ă��邩
		bool _imageType;		//!< �C���[�W�^�C�v
		bool _end;					//!< �������I�����邩
		Vector2 _startPos;	//!< �J�n�n�_
		/**
		 * @brief					�P�x�̉��Z����
		 * @return				�P�x������ɒB�����ꍇ��true��Ԃ�
		 *								���B���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool AddPal();
		/**
		 * @brief					�P�x�̌��Z����
		 * @return				�P�x�������ɓ��B���Ă���ꍇ��true��Ԃ�
		 *								���B���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool SubPal();
	public:
		/**
		 * @brief					�R���X�g���N�^
		 * @param game		�Q�[���N���X�̎Q��
		 */
		Particle_Image(Game& game);
		/**
		 * @brief					�f�X�g���N�^
		 */
		~Particle_Image() = default;
		/**
		 * @brief					����������
		 */
		void Init() override;
		/**
		 * @brief					�X�V����
		 */
		virtual void Process() override;
		/**
		 * @brief					�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief					�摜�̐ݒ�
		 * @param graph		�摜�L�[
		 * @param pos			�������W
		 * @param pal			�P�x
		 * @param extrate	�g�嗦
		 */
		void SetParameter(std::string graph, Vector2 pos, int pal = 0, double extrate = 1.0);
		/**
		 * @brief					�摜�̐ݒ�
		 * @param ivalue	�摜���
		 */
		void SetImage(ImageValue ivalue);
		/**
		 * @brief					�A�j���[�V��������
		 * @return				�A�j���[�V�����������I�������ꍇ��true��Ԃ�
		 *								����ȊO�̏ꍇ��false��Ԃ�
		 */
		bool Animation();
		/**
		 * @brief					�`��J�n
		 * @return				�����ɐ��������ꍇ��true��Ԃ�
		 *								���ɕ`�悪�J�n���Ă���ꍇ��false��Ԃ�
		 */
		virtual bool DrawStart();
		/**
		 * @brief					�`��I��
		 * @return				�����ɐ��������ꍇ��true��Ԃ�
		 *								���ɕ`�悪�I�����Ă���ꍇ��false��Ԃ�
		 */
		bool DrawEnd();
		/**
		 * @brief					�I������
		 * @return				�I���ɐ��������ꍇ��true��Ԃ�
		 *								���ɏI���������s���Ă���ꍇ��false��Ԃ�
		 */
		bool End();
		/**
		 * @brief					�C���[�W�^�C�v�̎擾
		 * @return				�C���[�W�^�C�v��Ԃ�
		 */
		inline bool IsType() { return _imageType; }
		/**
		 * @brief					�`�揈���t���O�̎擾
		 * @return				�`�揈���t���O��Ԃ�
		 */
		inline bool IsDraw() { return _isDraw; }
		/**
		 * @brief					�ʏ폈�����̔���
		 * @return				�ʏ��Ԃ̏ꍇ��true��Ԃ�
		 *								����������ꍇ��false��Ԃ�
		 */
		bool IsNormal();
		virtual void ChangePosition(bool type);
	};
}

