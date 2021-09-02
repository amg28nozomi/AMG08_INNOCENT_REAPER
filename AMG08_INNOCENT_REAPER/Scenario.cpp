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
		// �e����̓o�^
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

		Init();	// �z�񏉊���
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
		//	}// ���o�����A�z�z����J��
		//}
		_scenarios.clear();
	}

	//Vector2 Scenario::SpawnPosition(std::string key) {
	//	auto scenario = _scenarios.find(_scenarioKey);
	//	// �Ăяo�����I�u�W�F�N�g�̏����擾
	//	auto datas = scenario->second.equal_range(key);
	//	// �C�e���[�^���񂵂ď������s��
	//	int no = 1;
	//	Vector2 start;
	//	auto osize = _objSize.find(key);
	//	for (auto&& objvalue = datas.first; objvalue != datas.second; ++objvalue) {
	//		// �J�E���^����v���邩�H
	//		if (osize->second.second == no) {
	//			// �������W�����o��
	//			start = objvalue->second.Position();
	//			++osize->second.first;	// �J�E���^�𑝂₷
	//			break;	// ���[�v���I��
	//		}
	//		++no;	// ��v���Ȃ������ꍇ�͏������J��Ԃ�
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
	//	// �g�p����V�i���I�����o��
	//	auto scenario = _scenarios.find(_scenarioKey);
	//	auto it = scenario->second;
	//	
	//	std::unordered_set<std::string> keys;
	//	// �o�^����Ă���L�[������o��
	//	for (auto data : it) keys.insert(data.first);
	//	// �e�L�[�̓o�^��(�T�C�Y)�ƃJ�E���^��
	//	for (auto k : keys) { 
	//		std::pair<int, int> value = std::make_pair(static_cast<int>(it.count(k)), 0);
	//		_objSize.emplace(k, value); 
	//	}
	//}

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

	bool Scenario::AddObjects(const std::string key) {
		auto it = _scenarios.find(key);
		// �o�^����Ă���I�u�W�F�N�g(ObjectBase����)�̐���
		for (auto ovalue : it->second) {
			auto classType = ovalue.ClassName();	// ��������I�u�W�F�N�g�͉��Ȃ̂��H
			switch (classType) {
			case oscenario::OBJ_PLAYER:
				// �v���C���[�̓o�^����ݒ�
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