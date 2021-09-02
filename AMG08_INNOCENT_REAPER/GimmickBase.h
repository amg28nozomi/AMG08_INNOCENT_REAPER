#pragma once
#include "ObjectBase.h"

namespace inr {

	namespace gimmick {
		// ���o�[
		namespace lever {
			constexpr auto KEY_LEVER = "lever";
			constexpr auto LEVER_SIZE = 140;

			// �T�E���h
		}

		// ��
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";
			constexpr auto BLOCK_SIZE = 140;

			// �T�E���h
		}

		// �N���X�^��
		namespace crystal {
			namespace type1 {
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal1_empty";
				constexpr auto KEY_CRYSTAL_RRD = "crystal1_red";
				constexpr auto KEY_CRYSTAL_BLUE = "crystal1_blue";
			}
			namespace type2 {
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal2_empty";
				constexpr auto KEY_CRYSTAL_RED = "crystal2_red";
				constexpr auto KEY_CRTSTAL_BLUE = "crystal2_blue";
			}

			constexpr auto CRYSTAL_SIZE = 140;
			// �T�E���h
		}

		// ��
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";
			constexpr auto KEY_DOOR_RED = "door_red";
			constexpr auto KEY_DOOR_BLUE = "door_blue";
			constexpr auto DOOR_SIZE = 280;

			// �T�E���h

		}
	}

	class GimmickBase : public ObjectBase {
	protected:
		enum class GimmickType {
			// ���o�[�A��A����
			LEVER, ROCK, CRYSTAL 
		};
		GimmickType _gimmick;
	public:
		GimmickBase(Game& game);
		~GimmickBase();

		virtual void Init() override;
		void Draw() override;
	};
}

