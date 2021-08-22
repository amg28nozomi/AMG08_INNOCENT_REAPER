#pragma once
#include "ObjectBase.h"

namespace inr {

	class SoulCursor : public ObjectBase {
	public:
		SoulCursor(Game& game);
		~SoulCursor() = default;

		void Init() override;
		void Process() override;		// XV
		void Draw() override;	// •`‰æ
	private:
		Vector2 _moveVector;
		void Move(int lever);	// “ü—Í^ˆÚ“®ˆ—
		void PositionUpdate();
	};
}

