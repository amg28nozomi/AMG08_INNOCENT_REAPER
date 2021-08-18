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
		//	}// ���o�����A�z�z����J��
		//}
		_scenarios.clear();
	}

	Vector2 Scenario::SpawnPosition(std::string key) {
		auto scenario = _scenarios.find(_scenarioKey);
		// �Ăяo�����I�u�W�F�N�g�̏����擾
		auto datas = scenario->second.equal_range(key);
		// �C�e���[�^���񂵂ď������s��
		int no = 1;
		Vector2 start;
		auto osize = _objSize.find(key);
		for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
			// �J�E���^����v���邩�H
			if (osize->second.second == no) {
				// �������W�����o��
				start = objvalue->second.Position();
				++osize->second.first;	// �J�E���^�𑝂₷
				break;	// ���[�v���I��
			}
			++no;	// ��v���Ȃ������ꍇ�͏������J��Ԃ�
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
		// �g�p����V�i���I�����o��
		auto scenario = _scenarios.find(_scenarioKey);
		auto it = scenario->second;
		
		std::unordered_set<std::string> keys;
		// �o�^����Ă���L�[������o��
		for (auto data : it) keys.insert(data.first);
		// �e�L�[�̓o�^��(�T�C�Y)�ƃJ�E���^��
		for (auto k : keys) { 
			std::pair<int, int> value = std::make_pair(static_cast<int>(it.count(k)), 0);
			_objSize.emplace(k, value); 
		}
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