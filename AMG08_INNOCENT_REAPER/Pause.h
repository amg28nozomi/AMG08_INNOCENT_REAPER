/*****************************************************************//**
 * @file   Pause.h
 * @brief  �|�[�Y�Ǘ��p�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include <memory>
#include "Particle_Image.h"
#include "Image.h"

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class Game;
	/** �|�[�Y�N���X */
	class Pause {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Pause(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~Pause();
		/**
		 * @brief				����������
		 */
		void Init();
		/**
		 * @brief				�X�V����
		 */
		void Process();
		/**
		 * @brief				�`�揈��
		 */
		void Draw();
		/**
		 * @brief				������Ԃ̔���
		 * @return			������Ԃ̏ꍇ��true��Ԃ�
		 *							�񊈓���Ԃ̏ꍇ��false��Ԃ�
		 */
		bool IsActive();
		/**
		 * @brief				�����t���O�̎擾
		 * @return			�����t���O��Ԃ�
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																				//!< �Q�[���N���X�̎Q��
		std::vector<std::shared_ptr<Particle_Image>> _uis;	//!< �e��UI
		bool _active;																				//!< ���������Ă��邩
		bool _isEnd;																				//!< �|�[�Y���I�����邩�H
		bool _input;																				//!< ���͂��󂯕t���Ă��邩
		/**
		 * @brief				�A�i���O�X�e�B�b�N�̓��͏���
		 * @return	
		 */
		bool InputLever();
		/**
		 * @brief				�{�^���̓��͏���
		 * @return			�����ɐ��������ꍇ��true��Ԃ�
		 *							���sor���͂��Ȃ��ꍇ��false��Ԃ�
		 */
		bool InputButton();
		/**
		 * @brief				�|�[�Y���N�����邩
		 * @return			�N���ɐ��������ꍇ��true��Ԃ�
		 *							���s�����ꍇ��false��Ԃ�
		 */
		bool PauseOn();
		/**
		 * @brief				���ʉ��̍Đ�
		 */
		void Sound(std::string soundkey);
	};
}

