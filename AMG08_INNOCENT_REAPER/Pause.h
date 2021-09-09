#pragma once
#include <memory>

namespace inr {

	class Game;
	class Particle_Image;
	// �|�[�Y�@�\����p
	class Pause {
	public:
		Pause(Game& game);
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool IsActive() { return _active; }
	private:
		Game& _game;
		std::unique_ptr<Particle_Image> _bg;	// �o�b�N�O���E���h
		std::unique_ptr<Particle_Image> _operation;	// ��������p�摜
		std::unique_ptr<> _ui;	// �e��UI

		bool _active;	// ���������Ă��邩
		bool IsActive();	// ������Ԃɓ����Ă��邩�H
		bool PauseOn();	// �L�[���͂͂��������ǂ���

		// �K�v�Ȃ���
		// �w�i*
		// UI3��
		// �J�[�\��
		// ��������摜*
	};
}

