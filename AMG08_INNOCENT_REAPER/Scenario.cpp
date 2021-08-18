#include "Scenario.h"
#include "Game.h"
#include <unordered_set>
#include <string>
#include <algorithm>

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

	void Scenario::LoadObjectData(std::string key ,ObjectData& odata) {
		_scenarios.emplace(key, odata);

		//for (auto&& obj : odata) {
		//	//auto key = obj.first;
		//	_scenarios.emplace(key, obj.second);
		//}
	}

	void Scenario::ClearScenario() {
		for (auto&& scenario : _scenarios) {
			auto it = _scenarios.find(scenario.first);
			it->second.clear();
			/*for (auto&& ovalue = odata.first; ovalue != odata.second; ++ovalue) {
				ovalue->second.clear();
			}*/
		}
		//for (auto&& scenario : _scenarios) {
		//	auto it = _scenarios.equal_range(scenario.first);
		//	for (auto&& ovalue = it.first; ovalue != it.second; ++ovalue) {
		//		ovalue->second.clear();
		//	}// 取り出した連想配列を開放
		//}
		_scenarios.clear();
	}

	Vector2 Scenario::SpawnPosition(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		// 呼び出したオブジェクトの情報を取得
		auto datas = scenario->second.equal_range(key);
		// イテレータを回して処理を行う
		int no = 1;
		Vector2 start;
		auto osize = _objSize.find(key);
		for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
			// カウンタが一致するか？
			if (osize->second.second == no) {
				// 初期座標を取り出す
				start = objvalue->second.Position();
				++osize->second.first;	// カウンタを増やす
				break;	// ループを終了
			}
			++no;	// 一致しなかった場合は処理を繰り返す
		}
		return start;
	}

	bool Scenario::SoulState(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		auto datas = scenario->second.equal_range(key);
		int no = 1;

		return true;
	}

	void Scenario::SetObjectSize() {
		// 使用するシナリオを取り出す
		auto scenario = _scenarios.find(_scenarioKey);
		auto it = scenario->second;
		
		std::unordered_set<std::string> keys;
		// 登録されているキーを割り出す
		for (auto data : it) keys.insert(data.first);
		// 各キーの登録数(サイズ)とカウンタを
		for (auto k : keys) { 
			std::pair<int, int> value = std::make_pair(static_cast<int>(it.count(k)), 0);
			_objSize.emplace(k, value); 
		}
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