#pragma once
#include "Particle_Image.h"


namespace inr {

	namespace system {
		constexpr auto CURSOR_MOVE = "cursor_move";
	}

	class Pause_UI : public Particle_Image {
	public:
		Pause_UI(Game& game);
		~Pause_UI() = default;

		void Process() override;
		void ChangePosition(bool type) override;
	private:
		int _no;	// ���݂���ꏊ�͂ǂ���
		int _upDown;	// �㉺�ǂ���Ɉړ����邩�i1:��A-1:���j
		double _nextY;	// �ړ���
		bool _move;	// �ړ������ǂ����iTRUE�̎��̂݁A�ړ�������t�j
	};
}

