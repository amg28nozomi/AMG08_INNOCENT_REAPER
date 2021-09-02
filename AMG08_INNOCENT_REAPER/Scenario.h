#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Vector2.h"
#include "ObjectBase.h"


// 検索用キー
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
		constexpr auto OBJ_BLOCK = 6;
		constexpr auto OBJ_CRYSTAL = 7; // quartz
	}

	class Game;
	class Player;
	class EnemyBase;

	// 登録情報
	class ObjectValue {
	public:
		ObjectValue(int classtype, Vector2 xy, int soulcolor = 0, int gimmcktype = 0);
		ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor = 0, int gimmicktype = 0);
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline int GimmickType() { return _gimmickType; }
		inline std::vector<Vector2> Positions() { return _spawnPos; }
	private:
		int _class;	// 生成するクラスは何か
		std::vector<Vector2> _spawnPos;	// 出現地点
		int _soulType;	// 魂の有無（0:未所持　1:赤　2:青）
		int _gimmickType;	// ギミックの種類（-1:対象外　0:レバー　1:水晶　2:　3:ドア）
	};

	class Scenario {
	public:
		using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
		Scenario(Game& game);
		~Scenario();

		void Init();	// 初期化処理
		void LoadObjectData(std::string key, std::vector<ObjectValue> ovalues);	// 情報読み込み

		bool AddObjects(const std::string key);	// オブジェクトの生成

	private:

		Game& _game;
		ScenarioMap _scenarios;
		// ObjectSize _objSize;	// 登録されているキーのカウンタ

		void AddPlayer(ObjectValue ovalue);
		void AddEnemy(ObjectValue ovalue);

		void AddSoldierDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddBigDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddCrowDoll(ObjectBase* obj, ObjectValue ovalue);
		void AddSoul(ObjectBase* obj, ObjectValue ovalue);
		void AddLever(ObjectValue ovalue);
		void AddQuarts(ObjectBase* obj, ObjectValue ovalue);

		void ClearScenario();	// 連想配列の初期化
		//void CheckSize(const char* objkey);	// この
	};
}

