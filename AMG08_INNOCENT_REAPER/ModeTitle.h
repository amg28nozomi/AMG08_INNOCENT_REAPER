/*****************************************************************//**
 * @file   ModeTitle.h
 * @brief  �^�C�g�����Ǘ����郂�[�h�^�C�g���N���X�i���[�h�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class Logo;
	/** ���[�h�^�C�g�� */
	class ModeTitle : public ModeBase{
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		ModeTitle(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ModeTitle();
		/**
		 * @brief	����������
		 */
		virtual void Init();
		/**
		 * @brief �X�V����
		 */
		virtual void Process();
		/**
		 * @brief �`�揈��
		 */
		virtual void Draw();
	private:
		std::unique_ptr<Logo> _titleGh;	//!< �^�C�g�����S
	};
}