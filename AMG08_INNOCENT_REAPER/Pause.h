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
		Pause(Game& game);
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool IsActive();	// ������Ԃɓ����Ă��邩�H
		bool Active() { return _active; }
	private:
		Game& _game;
		std::vector<std::shared_ptr<Particle_Image>> _uis;	// �e��UI

		bool _active;	// ���������Ă��邩
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

