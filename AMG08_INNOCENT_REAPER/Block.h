#pragma once
#include "GimmickBase.h"

namespace inr {

	namespace gimmick {
		namespace block {
			constexpr auto BRAKE_ON = true;
			constexpr auto BRAKE_OFF = false;
		}
	}
	// ‰ó‚ê‚éŠâ
	class Block : public GimmickBase {
	public:
		Block(Game& game);
		~Block() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;
		void ObjValueUpdate() override;

		inline bool IsBreak() { return _break; }
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);
		bool Break();
	private:
		int _pal;	// “§–¾“x
		bool _break;	// ‰ó‚³‚ê‚½‚©”Û‚©
	};
}

