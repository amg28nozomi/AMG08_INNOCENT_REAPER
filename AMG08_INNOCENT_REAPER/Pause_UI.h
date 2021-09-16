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
		int _no;	// 現在いる場所はどこか
		int _upDown;	// 上下どちらに移動するか（1:上、-1:下）
		double _nextY;	// 移動先
		bool _move;	// 移動中かどうか（TRUEの時のみ、移動処理受付）
	};
}

