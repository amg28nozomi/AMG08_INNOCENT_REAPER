#pragma once
#include <unordered_map>
#include <memory>
#include "Vector2.h"
#include "ObjectBase.h"


// �����p�L�[
namespace objtype {
	constexpr auto PLAYER = "player";
	constexpr auto SOLDIER_DOLL = "soldier_doll";
	constexpr auto SOUL = "soul";
}

namespace inr {

	namespace oscenario {
		constexpr auto OBJ_PLAYER = 0;
		constexpr auto OBJ_SOLDIER_DOLL = 1;
		constexpr auto OBJ_BIG_DOLL = 2;
		constexpr auto OBJ_CROW_DOLL = 3;
		constexpr auto OBJ_SOUL = 4;
		constexpr auto OBJ_LEVER = 5;
		constexpr auto OBJ_QUARTS = 6; // quartz
	}

	class Game;
	class Player;
	class EnemyBase;

	// �o�^���
	class ObjectValue {
	public:
		ObjectValue(int classtype, Vector2 xy, int soulcolor = 0);
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline Vector2 Position() { return _spawnPos; }
	private:
		int _class;	// ��������N���X�͉���
		Vector2 _spawnPos;	// �o���n�_
		int _soulType;	// ���̗L���i0:�������@1:�ԁ@2:�j
	};

	class Scenario {
	public:
		using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
		Scenario(Game& game);
		~Scenario();

		void Init();	// ����������
		void LoadObjectData(std::string key, std::vector<ObjectValue> ovalues);	// ���ǂݍ���

		bool AddObjects(const std::string key);	// �I�u�W�F�N�g�̐���

	private:

		Game& _game;
		ScenarioMap _scenarios;
		// ObjectSize _objSize;	// �o�^����Ă���L�[�̃J�E���^

		void AddPlayer(ObjectValue ovalue);
		void AddEnemy(ObjectValue ovalue);

		void AddSoldierDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddBigDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddCrowDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddSoul(ObjectBase* obj, ObjectValue ovalue);
		void AddLever(ObjectBase* obj, ObjectValue ovalue);
		void AddQuarts(ObjectBase* obj, ObjectValue ovalue);

		void ClearScenario();	// �A�z�z��̏�����
		//void CheckSize(const char* objkey);	// ����
	};
}

