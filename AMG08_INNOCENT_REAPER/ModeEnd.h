/*****************************************************************//**
 * \file   ModeEnd.h
 * \brief  �G���f�B���O���Ǘ����郂�[�h�G���h�N���X�i���[�h�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {
	// �G���f�B���O
	class ModeEnd : public ModeBase {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		/**
		 * �R���X�g���N�^.
		 * 
		 * \param game
		 */
		ModeEnd(Game& game);
		// �f�X�g���N�^
		~ModeEnd() = default;
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
	private:
		int _count;								// �҂�����
		bool _end;								// �������I�����邩�H
		bool _input;							// ���͂��󂯕t���邩�H
		std::unique_ptr<MoveImage> _staffRoll;	// �X�^�b�t���[��
		// �Q�[���^�C�g���ɑJ�ڂ��邩�̔���
		bool IsEnd();
		// ���͍͂s��ꂽ��
		bool IsInput();
	};
}

