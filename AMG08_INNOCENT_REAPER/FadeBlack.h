/*****************************************************************//**
 * \file   FadeBlack.h
 * \brief  �t�F�[�h�C���E�t�F�[�h�A�E�g���s���t�F�[�h�u���b�N�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {
	/** �摜�̃R���p�C�����萔 */
	namespace image {
		// �摜�L�[
		constexpr auto BLACK = "black";		//!< ���摜�̃L�[
		// �Ó]�t���O
		constexpr auto FADE_OUT = true;		//!< �t�F�[�h�A�E�g
		constexpr auto FADE_IN = false;		//!< �t�F�[�h�C��
	}
	/** �t�F�[�h�u���b�N�N���X */
	class FadeBlack : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		FadeBlack(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~FadeBlack();
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
		 * 
		 */
		void Draw() override;
		/**
		 * @brief						�t�F�[�h�C���E�t�F�[�h�A�E�g�؂�ւ�����
		 * @param fadeType	�t�F�[�h�C���E�t�F�[�h�A�E�g�̂ǂ�����s����
		 *									true:�t�F�[�h�A�E�g�@false:�t�F�[�h�C��
		 * @param interval	�������J�n����܂ł̃C���^�[�o��
		 */
		void FlagChange(bool fadeType, int interval = 0);
		/**
		 * @brief		�I���t���O�̎擾
		 * @return	�I���t���O��Ԃ�
		 */
		inline bool IsEnd() { return _end; }
		/**
		 * @brief		���Z�E���Z�������I�����Ă��邩�̎擾
		 * @return	���������s���Ă���ꍇ��true��Ԃ�
		 *					���s���̏ꍇ��false��Ԃ�
		 */
		inline bool PalChange() { return _addEnd; }
	private:
		int _pal;					//!< �P�x
		int _interval;		//!< �����I����̑҂�����
		bool _isInterval;	//!< ���݂͑҂����Ԃ��H
		bool _end;				//!< �����͏I��������
		bool _type;				//!< ���Z�E���Z�����̂ǂ�����s����
		bool _addEnd;			//!< ���Z�E���Z�������I�����Ă��邩
		/**
		 * @brief		�t�F�[�h�A�E�g����
		 */
		void FadeOut();
		/**
		 * @brief		�t�F�[�h�C������
		 */
		void FadeIn();
		/**
		 * @brief		�������I�����Ă��邩�̔���
		 * @return	�I�����Ă���ꍇ��true��Ԃ�
		 *					�I�����Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool FadeEnd();
	};
}

