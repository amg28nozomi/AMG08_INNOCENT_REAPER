#include "Scenario.h"
#include "Game.h"
#include "ObjectServer.h"
#include <unordered_set>
#include <string>
#include <algorithm>


#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "GimmickBase.h"
#include "Lever.h"

namespace {
	constexpr auto COUNT_MIN = 0;
}

namespace inr {

	ObjectValue::ObjectValue(int classtype, Vector2 xy, int soulcolor, int gimmicktype) {
		// 各種情報の登録
		_class = classtype;
		_spawnPos.emplace_back(xy);
		_soulType = soulcolor;
		_gimmickType = gimmicktype;
	}

	ObjectValue::ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor, int gimmicktype) {
		_class = classtype;
		_spawnPos = xy;
		_soulType = soulcolor;
		_gimmickType = gimmicktype;
	}

	Scenario::Scenario(Game& game) : _game(game) {

		Init();	// 配列初期化
	}

	Scenario::~Scenario() {
		ClearScenario();
	}

	void Scenario::Init() {
		_scenarios.clear();
	}

	void Scenario::LoadObjectData(std::string key , std::vector<ObjectValue> ovalues) {
		auto it = _scenarios.find(key);
		if (it == _scenarios.end()) {
			_scenarios.emplace(key, ovalues);
		}

		//for (auto&& obj : odata) {
		//	//auto key = obj.first;
		//	_scenarios.emplace(key, obj.second);
		//}
	}

	void Scenario::ClearScenario() {
		//for (auto&& scenario : _scenarios) {
		//	auto it = _scenarios.equal_range(scenario.first);
		//	for (auto&& ovalue = it.first; ovalue != it.second; ++ovalue) {
		//		ovalue->second.clear();
		//	}// 取り出した連想配列を開放
		//}
		_scenarios.clear();
	}

	//Vector2 Scenario::SpawnPosition(std::string key) {
	//	auto scenario = _scenarios.find(_scenarioKey);
	//	// 呼び出したオブジェクトの情報を取得
	//	auto datas = scenario->second.equal_range(key);
	//	// イテレータを回して処理を行う
	//	int no = 1;
	//	Vector2 start;
	//	auto osize = _objSize.find(key);
	//	for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
	//		// カウンタが一致するか？
	//		if (osize->second.second == no) {
	//			// 初期座標を取り出す
	//			start = objvalue->second.Position();
	//			++osize->second.first;	// カウンタを増やす
	//			break;	// ループを終了
	//		}
	//		++no;	// 一致しなかった場合は処理を繰り返す
	//	}
	//	return start;
	//}

	/*bool Scenario::SoulState(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		auto datas = scenario->second.equal_range(key);
		int no = 1;

		return true;
	}*/

	//void Scenario::SetObjectSize() {
	//	// 使用するシナリオを取り出す
	//	auto scenario = _scenarios.find(_scenarioKey);
	//	auto it = scenario->second;
	//	
	//	std::unordered_set<std::string> keys;
	//	// 登録されているキーを割り出す
	//	for (auto data : it) keys.insert(data.first);
	//	// 各キーの登録数(サイズ)とカウンタを
	//	for (auto k : keys) { 
	//		std::pair<int, int> value = std::make_pair(static_cast<int>(it.count(k)), 0);
	//		_objSize.emplace(k, value); 
	//	}
	//}

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

	bool Scenario::AddObjects(const std::string key) {
		auto it = _scenarios.find(key);
		// 登録されているオブジェクト(ObjectBase限定)の生成
		for (auto ovalue : it->second) {
			auto classType = ovalue.ClassName();	// 生成するオブジェクトは何なのか？
			switch (classType) {
			case oscenario::OBJ_PLAYER:
				// プレイヤーの登録情報を設定
				AddPlayer(ovalue);
				break;
			case oscenario::OBJ_SOLDIER_DOLL:
				AddEnemy(ovalue);
				break;
			case oscenario::OBJ_LEVER:
				AddLever(ovalue);
			}
		}
		return false;
	}

	void Scenario::AddPlayer(ObjectValue ovalue) {
		auto player = std::make_shared<Player>(_game.GetGame());
		player->SetParameter(ovalue.Positions()[0]);
		_game.GetObjectServer()->Add(std::move(player));
	}

	/*void Scenario::AddPlayer(std::shared_ptr<ObjectBase> obj, ObjectValue ovalue) {
		auto player = std::dynamic_pointer_cast<Player>(obj);
		player->
	}*/

	void Scenario::AddEnemy(ObjectValue ovalue) {
		auto enemy = std::make_shared<SoldierDoll>(_game.GetGame());
		enemy->SetParameter(ovalue.Positions()[0], ovalue.SoulType());
		_game.GetObjectServer()->Add(std::move(enemy));
		
	}

	void Scenario::AddSoldierDoll(ObjectBase* obj, ObjectValue ovalue) {
	}

	void Scenario::AddBigDoll(ObjectBase* obj, ObjectValue ovalue) {

	}

	void Scenario::AddCrowDoll(ObjectBase* obj, ObjectValue ovalue) {

	}

	void Scenario::AddSoul(ObjectBase* obj, ObjectValue ovalue) {

	}

	void Scenario::AddLever(ObjectValue ovalue) {
		auto glever = std::make_shared<Lever>(_game.GetGame());
		auto posv = ovalue.Positions();
		glever->SetParameter(posv[0], posv[1], ovalue.GimmickType());
		_game.GetObjectServer()->Add(std::move(glever));
	}

	void Scenario::AddQuarts(ObjectBase* obj, ObjectValue ovalue) {

	}

}