#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;
	}

	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();

		void Init() override;
		void Process() override;
		void Draw() override;
	private:

	};
}

