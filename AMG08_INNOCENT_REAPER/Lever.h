#pragma once
#include "GimmickBase.h"

namespace inr {

	namespace gimmick {
		namespace door {
			constexpr auto NOT_DOOR = -1;
			constexpr auto D_LEVER = 1;
			constexpr auto D_RED = 2;
			constexpr auto D_BLUE = 3;
		}
	}

	class Door;

	class Lever : public GimmickBase {
	public:
		Lever(Game& game);
		~Lever() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(Vector2 spwanL, Vector2 spwanD, int doorno);	// ����1:���W(���o�[)�@����2;���W(�h�A)
	private:
		std::shared_ptr<Door> _door;
	};

}