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
	class TutorialValue;
	class Particle_Image;
	class TutorialImage;

	using ImageMap = std::unordered_map<int, ImageValue>;

	class Scenario {
	public:
		using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Scenario(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Scenario();

		void Init();	// ����������
		void ScenarioUpdate(std::string key);	// �V�i���I���X�V����

		bool AddObjects(const std::string key);	// �I�u�W�F�N�g�̐���

		void BossBlock();	// �{�X��J�n���Ƀu���b�N�𐶐�����
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
		void AddItem(ObjectValue ovalue);

		std::unique_ptr<Particle_Image> AddImage(ImageValue pvalue);
		bool AddTutorials(const std::string key);

		void LoadObjectData(std::string key, std::vector<ObjectValue> ovalues);	// ���ǂݍ���
		void LoadImageData(ImageMap imap);
		bool IsLoad(const std::string key);	// �Ώۂ̃L�[�͓ǂݍ��񂾂��H
		void ClearScenario();	// �A�z�z��̏�����
		//void CheckSize(const char* objkey);	// ����
	};
}

