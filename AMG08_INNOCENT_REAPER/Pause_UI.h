#pragma once
#include "Particle_Image.h"


namespace inr {

	namespace system {
		constexpr auto CURSOR_MOVE = "cursor_move";

		constexpr auto CONTINUE = 0;
		constexpr auto CONTROLS = 1;
		constexpr auto QUIT_TO_TITLE = 2;
	}

	class Pause_UI : public Particle_Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Pause_UI(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Pause_UI() = default;

		void Init() override;
		void Process() override;
		void ChangePosition(bool type) override;

		bool IsMove() { return _move; }
		int UiNumber() { return _no; }
	private:
		int _no;	// ���݂���ꏊ�͂ǂ���
		int _upDown;	// �㉺�ǂ���Ɉړ����邩�i1:��A-1:���j
		double _nextY;	// �ړ���
		bool _move;	// �ړ������ǂ����iTRUE�̎��̂݁A�ړ�������t�j
	};
}

