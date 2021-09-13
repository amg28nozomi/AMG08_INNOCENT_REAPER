#pragma once
#include "GimmickBase.h"

namespace inr {

	namespace gimmick {
		namespace door {
			constexpr auto NOT_DOOR = -1;
			constexpr auto D_LEVER = 1;
			constexpr auto D_RED = 2;
			constexpr auto D_BLUE = 3;
			constexpr auto D_BOSS = 4;
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

		void OpenDoor();
		void SetParameter(ObjectValue objValue) override;	// ����1:���W(���o�[)�@����2;���W(�h�A)
		void ObjValueUpdate() override;
	private:
		std::shared_ptr<Door> _door;
	};

}