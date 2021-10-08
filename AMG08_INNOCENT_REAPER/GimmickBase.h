/*****************************************************************//**
 * \file   GimmickBase.h
 * \brief  �M�~�b�N�̃X�[�p�[�N���X�i�I�u�W�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
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


		// ���o�[
		namespace lever {
			constexpr auto KEY_LEVER = "lever";
			constexpr auto KEY_LEVER_BOSS = "boss_lever";
			constexpr auto LEVER_SIZE = 140;
			constexpr auto BOSS_LEVER_SIZE = 200;
			// �T�E���h
			constexpr auto SE_LEVER = 50;
		}

		// ��
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";
			constexpr auto KEY_BREAK = "break_key";
			constexpr auto BLOCK_SIZE_W = 140;
			constexpr auto BLOCK_SIZE_H = 280;
			constexpr auto BRAKE_SIZE = 500;
			// �T�E���h
			constexpr auto SE_BREAK = 50;
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
			constexpr auto KEY_CRYSTAL = "crystal_se";
			constexpr auto SE_CRYSTAL = 40;
			// �������ꂽ
			constexpr auto KEY_ROB = "crystal_rob_se";
			// ����^����ꂽ��
			constexpr auto KEY_GIVE = "crystal_give_se";
		}

		// ��
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";
			constexpr auto KEY_DOOR_BOSS = "door_boss";
			constexpr auto KEY_DOOR_RED = "door_red";
			constexpr auto KEY_DOOR_BLUE = "door_blue";
			constexpr auto DOOR_SIZE = 280;
			// �T�E���h
			constexpr auto KEY_DOOR = "door_se";
			constexpr auto SE_OPEN = 40;

			constexpr auto SE_CLOSE_DOOR = "door_close";	// �����܂鉹
		}
	}

	class GimmickBase : public ObjectBase {
	protected:
		// �M�~�b�N�̎��(�񋓌^)
		enum class GimmickType {
			// ���o�[�A��A����
			NOT_GIMMICK, LEVER, BLOCK, CRYSTAL, DOOR
		};
		GimmickType _gType;
	public:
		GimmickBase(Game& game);
		~GimmickBase();

		virtual void Init() override;
		virtual void Process() override;
		virtual void Draw() override;

		virtual void ObjValueUpdate();	// �t���O�ƍ���؂�ւ���

		int GimmickType();

		// virtual bool HitCollision();	// �Փ˂������H

	};
}

