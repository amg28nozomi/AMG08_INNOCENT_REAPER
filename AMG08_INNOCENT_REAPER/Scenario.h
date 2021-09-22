#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Vector2.h"


// 検索用キー
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

		void Init();	// 初期化処理
		void ScenarioUpdate(std::string key);	// シナリオを更新する

		bool AddObjects(const std::string key);	// オブジェクトの生成
	private:

		Game& _game;
		ScenarioMap _scenarios;
		// ObjectSize _objSize;	// 登録されているキーのカウンタ

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

		void LoadObjectData(std::string key, std::vector<ObjectValue> ovalues);	// 情報読み込み
		bool IsLoad(const std::string key);	// 対象のキーは読み込んだか？
		void ClearScenario();	// 連想配列の初期化
		//void CheckSize(const char* objkey);	// この
	};
}

