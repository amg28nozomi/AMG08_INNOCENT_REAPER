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
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Pause_UI(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Pause_UI() = default;

		void Init() override;
		void Process() override;
		void ChangePosition(bool type) override;

		bool IsMove() { return _move; }
		int UiNumber() { return _no; }
	private:
		int _no;	// 現在いる場所はどこか
		int _upDown;	// 上下どちらに移動するか（1:上、-1:下）
		double _nextY;	// 移動先
		bool _move;	// 移動中かどうか（TRUEの時のみ、移動処理受付）
	};
}

