#pragma once
#include <unordered_map>
#include "Vector2.h"

// 検索用キー
namespace objtype {
	constexpr auto PLAYER = "player";
	constexpr auto SOLDIER_DOLL = "soldier_doll";
	constexpr auto SOUL = "soul";
}

namespace inr {

	class Game;

	// 登録情報
	class ObjectValue {
	public:
		ObjectValue(Vector2 xy);
		~ObjectValue() = default;

		Vector2 Position() { return _spawnPos; }
	private:
		Vector2 _spawnPos;	// 出現地点
		// 魂の有無
	};

	class Scenario {
	public:
		using ObjectData = std::unordered_multimap<std::string, ObjectValue>;	// 左辺:オブジェクトの種類、右辺:初期化情報

		Scenario(Game& game);
		~Scenario();

		void Init();	// 初期化処理
		void LoadObjectData(ObjectData& odata);	// 情報読み込み
		void ChangeKey(std::string nextScenario) { _scenarioKey = nextScenario; }	// 取り出すデータの切り替え
		
		Vector2 SpawnPosition(std::string key);	// 初期座標の取得
		bool SoulState(std::string key);	// 魂の取得
	private:
		using ScenarioMap = std::unordered_map<std::string, ObjectData>;	// 読み込み用
		using ObjectSize = std::unordered_map<std::string, std::pair<int, int>>;	// 登録されている各オブジェクトの(最大サイズ・カウンタ)

		Game& _game;
		ScenarioMap _scenarios;
		ObjectSize _objSize;	// 登録されているキーのカウンタ

		std::string _scenarioKey;	// 現在使用しているデータのキー
		int _objno;	// 読み込み番号

		void ClearScenario();	// 連想配列の初期化
		void SetObjectSize();
		//void CheckSize(const char* objkey);	// この
	};
}

