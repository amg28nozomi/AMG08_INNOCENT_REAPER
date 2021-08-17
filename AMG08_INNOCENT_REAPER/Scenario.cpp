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
		// �e����̓o�^
		_spawnPos = xy;
	}

	Scenario::Scenario(Game& game) : _game(game) {

		Init();	// �z�񏉊���
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
			}// ���o�����A�z�z����J��
		}
		_scenarios.clear();
	}

	Vector2 Scenario::SpawnPosition(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		// �Ăяo�����I�u�W�F�N�g�̏����擾
		auto datas = scenario->second.equal_range(key);
		// �C�e���[�^���񂵂ď������s��
		for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
			objvalue->second.Position();
		}
			//scenario->second.find(key);
		// auto spwanpos = datas->second.Position();	// �����n�_���擾


		Vector2 start = { 0,0 };
		return start;
	}
	bool Scenario::SoulState(std::string key) {
		return true;
	}

	void Scenario::SetObjectSize() {
		// �g�p����V�i���I�����o��
		auto scenario = _scenarios.find(_scenarioKey);
		auto it = scenario->second;
		// �o�^����Ă���L�[������o��
		std::unordered_set<std::string> keys;

		for (auto data : it) keys.insert(data.first);
		for (auto k : keys) _objSize.emplace(k, std::make_pair(it.count(k), 0));
	}

	//void Scenario::CheckSize(const char* objkey) {
	//	auto it = _objSize.find(objkey);
	//	// �q�b�g���Ȃ������ꍇ�̂݁A�o�^�������s��
	//	if (it == _objSize.end()) {
	//		auto setValue = std::make_pair(1, COUNT_MIN);
	//		_objSize.emplace(objkey, setValue);
	//		return;	// �����𔲂���
	//	}
	//	// �v�f���𑝂₷
	//	++it->second.first;
	//}
}