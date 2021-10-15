#pragma once
#include <vector>
#include "Vector2.h"

namespace inr {

	namespace oscenario {
		constexpr auto OBJ_NULL = -1;
		constexpr auto OBJ_PLAYER = 0;
		constexpr auto OBJ_SOLDIER_DOLL = 1;
		constexpr auto OBJ_BIG_DOLL = 2;
		constexpr auto OBJ_CROW_DOLL = 3;
		constexpr auto OBJ_SOUL = 4;
		constexpr auto OBJ_LEVER = 5;
		constexpr auto OBJ_BLOCK = 6;
		constexpr auto OBJ_CRYSTAL = 7; // quartz
		constexpr auto OBJ_DOOR = 8;
		constexpr auto OBJ_ITEM = 9;

		namespace type {
			constexpr auto PLAYER = 0;
			constexpr auto ENEMY = 1;
			constexpr auto SOUL = 2;
			constexpr auto GIMMICK = 3;
			constexpr auto ITEM = 4;
		}

		namespace gimmick {
			constexpr auto TYPE_NULL = -1;
			constexpr auto TYPE_LEVER = 0;
			constexpr auto TYPE_CRYSTAL = 1;
			constexpr auto TYPE_BLOCK = 2;
			constexpr auto TYPE_DOOR = 3;

			constexpr auto FLAG_NULL = -1;
			constexpr auto FLAG_FALSE = 0;
			constexpr auto FLAG_TRUE = 1;

			namespace crystal {
				constexpr auto TYPE_NULL = -1;
				constexpr auto TYPE_STAGE_0 = 0;
				constexpr auto TYPE_STAGE_2 = 1;

				constexpr auto DOOR_NULL = -1;
				constexpr auto DOOR_RED = 0;
				constexpr auto DOOR_BLUE = 1;
			}
		}
	}

	/** �M�~�b�N�̏�� */
	class GimmickValue {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 * @param �M�~�b�N�̎��
		 * @param �M�~�b�N�̃t���O
		 * @param �����̎��
		 * @param �h�A�̎��
		 */
		GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL, int ctype = oscenario::gimmick::crystal::TYPE_NULL, std::vector<int> types = { oscenario::gimmick::crystal::DOOR_NULL });
		inline int GimmickType() { return _gimmickType; }
		inline int GimmickFlag() { return _gimmickFlag; }
		inline int CrystalType() { return _crystalType; }
		inline std::vector<int> DoorType() { return _dtype; }
		inline void AddFlag(int flag) { _gimmickFlag = flag; }
	private:
		int _gimmickType;	// �M�~�b�N�̎�ށi-1:�ΏۊO�@0:���o�[�@1:�����@2:�@3:�h�A�j
		int _gimmickFlag;	// �M�~�b�N�̃t���O�i-1:�ΏۊO�@0:false�@1:true�j
		int _crystalType;	// �N���X�^���̎��(-1:�ΏۊO�A0:�X�e�[�W0�A1:�X�e�[�W2)
		std::vector<int> _dtype;	// �h�A�̎��

	};

	// �o�^���
	class ObjectValue {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 */
		ObjectValue();
		/**
		 * @brief	�R���X�g���N�^
		 * @param ��������N���X
		 * @param �����n�_
		 * @param ���̐F(0:�Ȃ��@1:�ԁ@2:��)
		 * @param �M�~�b�N���
		 */
		ObjectValue(int classtype, Vector2 xy, bool direction = false, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		/**
		 * @brief	�R���X�g���N�^
		 * @param ��������N���X
		 * @param �����n�_���i�[�����R���e�i
		 * @param ���̐F(0:�Ȃ��@1:�ԁ@2:��)
		 * @param �M�~�b�N���
		 */
		ObjectValue(int classtype, std::vector<Vector2> xy, bool direction = false, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline int GimmickType() { return _gimmick.GimmickType(); }
		inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
		inline int CrystalType() { return _gimmick.CrystalType(); }
		inline int ObjectType() { return _objType; }
		inline bool Direction() { return _direction; }
		inline std::vector<Vector2> Positions() { return _spawnPos; }
		inline std::vector<int> DoorType() { return _gimmick.DoorType(); }

		void PositionsUpdate(Vector2 newpos);
		void FlagUpdate(int flag, int soul = 0);
		inline void SoulUpdate(int soul) { _soulType = soul; }
		inline void DirectionUpdate(bool newdir) { _direction = newdir; }
		
	private:
		int _class;	// ��������N���X�͉���
		std::vector<Vector2> _spawnPos;	// �o���n�_
		bool _direction;	// ����
		int _soulType;	// ���̗L���i0:�������@1:�ԁ@2:�j
		GimmickValue _gimmick;	// �M�~�b�N�̓o�^���
		int _objType;	// �I�u�W�F�N�g�̎��

		int IsObjectType(int classname);	// ���̃I�u�W�F�N�g�͉����H
	};
}

