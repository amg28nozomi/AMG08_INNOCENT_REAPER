/*****************************************************************//**
 * @file   Logo.h
 * @brief  ���S�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {
	/** �^�C�g���̃R���p�C�����萔 */
	namespace titles {
		// �^�C�g���A�j���[�V�����̉摜�L�[
		constexpr auto TITLE_ALI = "title_logo_in";		//!< AMG���S�t�F�[�h�C��
		constexpr auto TITLE_ALO = "title_logo_out";	//!< AMG���S�t�F�[�h�A�E�g
		constexpr auto TITLE_T = "title_animation";		//!< �^�C�g����ʃA�j���[�V����
		constexpr auto TITLE_TLI = "team_logo_in";		//!< �`�[�����S�t�F�[�h�C��
		constexpr auto TITLE_TLO = "team_logo_out";		//!< �`�[�����S�t�F�[�h�A�E�g
	}
	/** ���S�N���X */
	class Logo : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Logo(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~Logo() = default;
		/**
		 * @brief				����������
		 */
		void Init() override;
		/**
		 * @brief				�X�V����
		 */
		void Process() override;
		/**
		 * @brief				�`�揈��
		 */
		void Draw() override;
	private:
		int _number;												//!< ���݉��Ԗڂ̏������s���Ă��邩�H
		int _fCount;												//!< �t���[���J�E���^
		int _maxFrame;											//!< �ő�Đ��t���[��
		int _allnum;												//!< �摜��
		int _wait;													//!< �҂�����
		bool _calculation;									//!< �U:�t�F�[�h�C���A�^:�t�F�[�h�A�E�g
		bool _animation;										//!< �A�j���[�V�������s�����ۂ�
		RedGreenBlue _rgb;									//!< RGB
		std::unique_ptr<TitleLogo> _tlogo;	//!< �^�C�g�����S
		/**
		 * @brief				���S�摜�̐؂�ւ�
		 */
		void ChangeGraph();
		/**
		 * @brief				�A�j���[�V�����̏�����
		 */
		void AnimationInit();
		/**
		 * @brief				�A�j���[�V�����̃X�L�b�v
		 */
		void AnimaSkip();
	};
}

