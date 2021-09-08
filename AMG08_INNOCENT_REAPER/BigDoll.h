#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;

		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;
	}

	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		void HipDrop();	// ヒップドロップ
		void StateUpdate();	// 状態に応じた処理を行う

		void Death() override;
	};
}

