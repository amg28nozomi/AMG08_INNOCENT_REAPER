/*****************************************************************//**
 * @file   ModeServer.h
 * @brief  �e�탂�[�h���Ǘ����郂�[�h�T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "FadeBlack.h"

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class Game;
	class ModeMain;
	/** ���[�h�T�[�o */
	class ModeServer {
	public:
		/**
		 * @brief					 �R���X�g���N�^
		 * @param game		 �Q�[���N���X�̎Q��
		 */
		ModeServer(Game& game);
		/**
		 * @brief					 �f�X�g���N�^
		 */
		~ModeServer();
		/**
		 * @brief					 ����������
		 */
		void Init();
		/**
		 * @brief					 �X�V����
		 */
		void Process();
		/**
		 * @brief					 �`�揈��
		 */
		void Draw();
		/**
		 * @brief					 ���[�h�̐؂�ւ�����
		 * @param necxMode ���̃��[�h
		 * @param	interval �J�ڂ܂łɗv����C���^�[�o��
		 */
		void ModeChange(std::string nextMode, int interval = 0);
		/**
		 * @brief					 �Q�[���̏I������
		 */
		void GameEnd();
		/**
		 * @brief					 �Q�[���{�҂̎擾
		 * @return				 �Q�[���{�҂̃V�F�A�[�h�|�C���^��Ԃ�
		 */
		std::shared_ptr<ModeMain> GetModeMain();
		/**
		 * @brief					 �t�F�[�h�A�E�g�����Ăяo��
		 */
		inline void FadeOut() { _fadeBlack->FlagChange(image::FADE_OUT, 60); }
		/**
		 * @brief					 ���Z�E���Z�������I�����Ă��邩�̔���
		 * @return				 �߂�l��Ԃ�
		 */
		inline bool PalChange() { return _fadeBlack->PalChange(); }
		/**
		 * @brief					 �I���t���O�̎擾
		 * @return				 �I���t���O��Ԃ�
		 */
		inline bool IsFadeEnd() { return _fadeBlack->IsEnd(); }
	private:
		/** ���[�h���i�[����A�z�z��(�l:�L�[�@�l:���[�h) */
		using ModeMap = std::unordered_map<std::string, std::shared_ptr<ModeBase>>;
		Game& _game;														//!< �Q�[���N���X�̎Q��
		ModeMap _modes;													//!< ���[�h�p�R���e�i
		std::unique_ptr<FadeBlack> _fadeBlack;	//!< �t�F�[�h
		std::string _modeKey;										//!< ���[�h�����p�L�[
		std::string _ChangeKey;									//!< ���[�h�؂�ւ��p�L�[
		/**
		 * @brief					 ���[�h�̊J��
		 */
		void ClearModeLists();
		/**
		 * @brief					 ���[�h�̐؂�ւ�
		 */
		void IsModeChange();
		/**
		 * @brief					 ���[�h�̏�����
		 * @return				 �������ɐ��������ꍇ��true��Ԃ�
		 *								 ���s�����ꍇ��false��Ԃ�
		 */
		bool ModeInit();
	};

}
