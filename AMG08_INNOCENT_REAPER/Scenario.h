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
		ObjectValue(Vector2 xy);
		~ObjectValue() = default;

		Vector2 Position() { return _spawnPos; }
	private:
		Vector2 _spawnPos;	// �o���n�_
		// ���̗L��
	};

	class Scenario {
	public:
		using ObjectData = std::unordered_multimap<std::string, ObjectValue>;	// ����:�I�u�W�F�N�g�̎�ށA�E��:���������

		Scenario(Game& game);
		~Scenario();

		void Init();	// ����������
		void LoadObjectData(ObjectData& odata);	// ���ǂݍ���
		void ChangeKey(std::string nextScenario) { _scenarioKey = nextScenario; }	// ���o���f�[�^�̐؂�ւ�
		
		Vector2 SpawnPosition(std::string key);	// �������W�̎擾
		bool SoulState(std::string key);	// ���̎擾
	private:
		using ScenarioMap = std::unordered_map<std::string, ObjectData>;	// �ǂݍ��ݗp
		using ObjectSize = std::unordered_map<std::string, std::pair<int, int>>;	// �o�^����Ă���e�I�u�W�F�N�g��(�ő�T�C�Y�E�J�E���^)

		Game& _game;
		ScenarioMap _scenarios;
		ObjectSize _objSize;	// �o�^����Ă���L�[�̃J�E���^

		std::string _scenarioKey;	// ���ݎg�p���Ă���f�[�^�̃L�[
		int _objno;	// �ǂݍ��ݔԍ�

		void ClearScenario();	// �A�z�z��̏�����
		void SetObjectSize();
		//void CheckSize(const char* objkey);	// ����
	};
}

