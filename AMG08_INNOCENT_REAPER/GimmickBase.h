#pragma once
#include "ObjectBase.h"

namespace inr {

	namespace gimmick {

		constexpr auto NOT_GIMMICK = -1;
		constexpr auto LEVER = 0;
		constexpr auto CRYSTAL = 1;
		constexpr auto BLOCK = 2;
		constexpr auto DOOR = 3;

		constexpr auto OFF = false;
		constexpr auto ON = true;


		// レバー
		namespace lever {
			constexpr auto KEY_LEVER = "lever";
			constexpr auto LEVER_SIZE = 140;
			// サウンド
			constexpr auto SE_LEVER = 50;
		}

		// 岩
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";
			constexpr auto BLOCK_SIZE = 140;
			// サウンド
			constexpr auto SE_BREAK = 50;
		}

		// クリスタル
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
			// サウンド
			constexpr auto KEY_CRYSTAL = "crystal_se";
			constexpr auto SE_CRYSTAL = 40;
		}

		// 扉
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";
			constexpr auto KEY_DOOR_RED = "door_red";
			constexpr auto KEY_DOOR_BLUE = "door_blue";
			constexpr auto DOOR_SIZE = 280;
			// サウンド
			constexpr auto KEY_DOOR = "door_se";
			constexpr auto SE_OPEN = 40;
		}
	}

	class GimmickBase : public ObjectBase {
	protected:
		enum class GimmickType {
			// レバー、岩、水晶
			LEVER, BLOCK, CRYSTAL, DOOR
		};
		GimmickType _gType;
		// bool _extrude;	// 押し出し処理を行うか？
	public:
		GimmickBase(Game& game);
		~GimmickBase();

		virtual void Init() override;
		virtual void Process() override;
		virtual void Draw() override;

		virtual void ObjValueUpdate();	// フラグと魂を切り替える

		int GimmickType();

		// virtual bool HitCollision();	// 衝突したか？

	};
}

