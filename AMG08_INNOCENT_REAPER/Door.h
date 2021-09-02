#pragma once
#include "GimmickBase.h"

namespace inr {

	class Door : public GimmickBase {
	public:
		Door(Game& game);
		~Door() = default;

		void Init() override;
		void Process() override;

		void SetParameter(Vector2 spwan, std::string key);
	private:
		enum class DoorType {
			// レバー、水晶(赤)、水晶(青)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
	};
}

