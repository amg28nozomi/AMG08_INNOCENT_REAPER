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

		namespace type {
			constexpr auto PLAYER = 0;
			constexpr auto ENEMY = 1;
			constexpr auto SOUL = 2;
			constexpr auto GIMMICK = 3;
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
		}
	}

	// �M�~�b�N�̏��
	class GimmickValue {
	public:
		GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL);
		inline int GimmickType() { return _gimmickType; }
		inline int GimmickFlag() { return _gimmickFlag; }
	private:
		int _gimmickType;	// �M�~�b�N�̎�ށi-1:�ΏۊO�@0:���o�[�@1:�����@2:�@3:�h�A�j
		int _gimmickFlag;	// �M�~�b�N�̃t���O�i-1:�ΏۊO�@0:false�@1:true�j
	};

	// �o�^���
	class ObjectValue {
	public:
		ObjectValue();
		ObjectValue(int classtype, Vector2 xy, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline int GimmickType() { return _gimmick.GimmickType(); }
		inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
		inline int ObjectType() { return _objType; }
		inline std::vector<Vector2> Positions() { return _spawnPos; }
		
	private:
		int _class;	// ��������N���X�͉���
		std::vector<Vector2> _spawnPos;	// �o���n�_
		int _soulType;	// ���̗L���i0:�������@1:�ԁ@2:�j
		GimmickValue _gimmick;	// �M�~�b�N�̓o�^���
		int _objType;	// �I�u�W�F�N�g�̎��

		int IsObjectType(int classname);	// ���̃I�u�W�F�N�g�͉����H
	};
}

