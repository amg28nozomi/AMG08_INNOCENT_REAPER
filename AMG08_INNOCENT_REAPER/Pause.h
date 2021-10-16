#pragma once
#include <vector>
#include <memory>
#include "Particle_Image.h"
#include "Image.h"

namespace inr {

	class Game;
	// �|�[�Y�@�\����p
	class Pause {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Pause(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool IsActive();	// ������Ԃɓ����Ă��邩�H
		/**
		 * @brief		�����t���O�̎擾
		 * @return	�����t���O��Ԃ�
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																				//!< �Q�[���N���X�̎Q��
		std::vector<std::shared_ptr<Particle_Image>> _uis;	//!< �e��UI
		bool _active;																				//!< ���������Ă��邩
		bool _isEnd;																				//!< �|�[�Y���I�����邩�H
		bool _input;																				//!< ���͂��󂯕t���Ă��邩
		bool InputLever();	// ���o�[
		bool InputButton();	// �{�^��
		bool PauseOn();	// �L�[���͂͂��������ǂ���

		void Sound(std::string soundkey);

		// �K�v�Ȃ���
		// �w�i*
		// UI3��
		// �J�[�\��
		// ��������摜*
	};
}

