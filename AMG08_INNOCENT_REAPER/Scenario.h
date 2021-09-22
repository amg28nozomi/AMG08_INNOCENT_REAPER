#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Vector2.h"


// �����p�L�[
namespace objtype {
	constexpr auto PLAYER = "player";
	constexpr auto SOLDIER_DOLL = "soldier_doll";
	constexpr auto SOUL = "soul";
}

namespace inr {

	

	class Game;
	class Player;
	class EnemyBase;
	class ObjectValue;
	class ImageValue;


	class Scenario {
	public:
		using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
		Scenario(Game& game);
		~Scenario();

		void Init();	// ����������
		void ScenarioUpdate(std::string key);	// �V�i���I���X�V����

		bool AddObjects(const std::string key);	// �I�u�W�F�N�g�̐���
	private:

		Game& _game;
		ScenarioMap _scenarios;
		// ObjectSize _objSize;	// �o�^����Ă���L�[�̃J�E���^

		void AddPlayer();
		void AddEnemy(ObjectValue ovalue);

		void AddSoldierDoll(ObjectValue ovalue);
		void AddBigDoll(ObjectValue ovalue);
		void AddCrowDoll(ObjectValue ovalue);
		void AddSoul(ObjectValue ovalue);
		void AddLever(ObjectValue ovalue);
		void AddCrystal(ObjectValue ovalue);
		void AddBlock(ObjectValue ovalue);
		void AddDoor(ObjectValue ovalue);

		void LoadObjectData(std::string key, std::vector<ObjectValue> ovalues);	// ���ǂݍ���
		bool IsLoad(const std::string key);	// �Ώۂ̃L�[�͓ǂݍ��񂾂��H
		void ClearScenario();	// �A�z�z��̏�����
		//void CheckSize(const char* objkey);	// ����
	};
}

