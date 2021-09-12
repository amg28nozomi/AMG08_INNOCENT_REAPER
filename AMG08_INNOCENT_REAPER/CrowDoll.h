#pragma once
#include "EnemyBase.h"

namespace inr {

	class CrowDoll : public EnemyBase {
		CrowDoll(Game& game);

		void Process() override;
		void Draw() override;

	};
}

