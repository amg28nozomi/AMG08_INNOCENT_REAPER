#pragma once
#include <unordered_map>
#include "Vector2.h"

// �����p�L�[
namespace objtype {
	constexpr auto PLAYER = "player";
	constexpr auto SOLDIER_DOLL = "soldier_doll";
	constexpr auto SOUL = "soul";
}

namespace inr {

	class Game;

	// �o�^���
	class ObjectValue {
	public:
		ObjectValue(int classtype, Vector2 xy);
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline Vector2 Position() { return _spawnPos; }
	private:
		int _class;	// ��������N���X�͉���
		Vector2 _spawnPos;	// �o���n�_
		int _soulType;	// ���̗L���i0:�������@1:�ԁ@2:�j
	};

	class Scenario {
	public:
		using ObjectData = std::unordered_multimap<std::string, ObjectValue>;	// ����:�I�u�W�F�N�g�̎�ށA�E��:���������

		Scenario(Game& game);
		~Scenario();

		void Init();	// ����������
		void LoadObjectData(std::string key, ObjectData& odata);	// ���ǂݍ���
		void ChangeKey(std::string nextScenario) { _scenarioKey = nextScenario; }	// ���o���f�[�^�̐؂�ւ�

		bool AddObjects();	// �I�u�W�F�N�g�̐���
		
		//Vector2 SpawnPosition(std::string key);	// �������W�̎擾
		//bool SoulState(std::string key);	// ���̎擾
	private:
		using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
		// using ScenarioMap = std::unordered_map<std::string, ObjectData>;	// �ǂݍ��ݗp
		// using ObjectSize = std::unordered_map<std::string, std::pair<int, int>>;	// �o�^����Ă���e�I�u�W�F�N�g��(�ő�T�C�Y�E�J�E���^)

		Game& _game;
		ScenarioMap _scenarios;
		// ObjectSize _objSize;	// �o�^����Ă���L�[�̃J�E���^

		std::string _scenarioKey;	// ���ݎg�p���Ă���f�[�^�̃L�[

		void AddPlayer();
		void AddSolderDoll();
		void AddBigDoll();
		void AddCrowDoll();
		void AddSoul();
		void AddLever();
		void AddQuarts();

		void ClearScenario();	// �A�z�z��̏�����
		//void CheckSize(const char* objkey);	// ����
	};
}

