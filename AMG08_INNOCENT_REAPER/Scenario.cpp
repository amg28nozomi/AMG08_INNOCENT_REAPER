#include "Scenario.h"
#include "Game.h"
#include <algorithm>
#include <unordered_set>
#include <string>

namespace {
	constexpr auto COUNT_MIN = 0;
}

namespace inr {

	ObjectValue::ObjectValue(Vector2 xy) {
		// 各種情報の登録
		_spawnPos = xy;
	}

	Scenario::Scenario(Game& game) : _game(game) {

		Init();	// 配列初期化
	}

	Scenario::~Scenario() {
		ClearScenario();
		_objSize.clear();
	}

	void Scenario::Init() {
		_scenarios.clear();
		_objSize.clear();
	}

	void Scenario::LoadObjectData(ObjectData& odata) {
		for (auto&& obj : odata) {
			auto key = obj.first;
			_scenarios.emplace(key, obj.second);
		}
	}

	void Scenario::ClearScenario() {
		for (auto&& scenario : _scenarios) {
			auto it = _scenarios.equal_range(scenario.first);
			for (auto&& ovalue = it.first; ovalue != it.second; ++ovalue) {
				ovalue->second.clear();
			}// 取り出した連想配列を開放
		}
		_scenarios.clear();
	}

	Vector2 Scenario::SpawnPosition(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		// 呼び出したオブジェクトの情報を取得
		auto datas = scenario->second.equal_range(key);
		// イテレータを回して処理を行う
		for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
			objvalue->second.Position();
		}
			//scenario->second.find(key);
		// auto spwanpos = datas->second.Position();	// 生成地点を取得


		Vector2 start = { 0,0 };
		return start;
	}
	bool Scenario::SoulState(std::string key) {
		return true;
	}

	void Scenario::SetObjectSize() {
		// 使用するシナリオを取り出す
		auto scenario = _scenarios.find(_scenarioKey);
		auto it = scenario->second;
		// 登録されているキーを割り出す
		std::unordered_set<std::string> keys;

		for (auto data : it) keys.insert(data.first);
		for (auto k : keys) _objSize.emplace(k, std::make_pair(it.count(k), 0));
	}

	//void Scenario::CheckSize(const char* objkey) {
	//	auto it = _objSize.find(objkey);
	//	// ヒットしなかった場合のみ、登録処理を行う
	//	if (it == _objSize.end()) {
	//		auto setValue = std::make_pair(1, COUNT_MIN);
	//		_objSize.emplace(objkey, setValue);
	//		return;	// 処理を抜ける
	//	}
	//	// 要素数を増やす
	//	++it->second.first;
	//}
}