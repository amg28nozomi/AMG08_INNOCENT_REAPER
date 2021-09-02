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
			// ���o�[�A����(��)�A����(��)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
	};
}

