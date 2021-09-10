#include "Scenario.h"
#include "Game.h"
#include "ObjectServer.h"
#include <unordered_set>
#include <string>
#include <algorithm>

#include "ObjectValue.h"
#include "Loads.h"

#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "BigDoll.h"
#include "GimmickBase.h"
#include "Lever.h"

namespace {
	constexpr auto COUNT_MIN = 0;
}

namespace inr {

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
	//	// �g�p����V�i���I�����o��7
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
		IsLoad(key);
		AddPlayer();
		auto it = _scenarios.find(key);
		// �o�^����Ă���I�u�W�F�N�g(ObjectBase����)�̐���
		for (auto ovalue : it->second) {
			auto classType = ovalue.ClassName();	// ��������I�u�W�F�N�g�͉��Ȃ̂��H
			switch (classType) {
			// �\���W���[�h�[���̐���
			case oscenario::OBJ_SOLDIER_DOLL:
				AddSoldierDoll(ovalue);
				continue;
			// �r�b�O�h�[���̐���
			case oscenario::OBJ_BIG_DOLL:
				AddBigDoll(ovalue);
				continue;
			// �N���E�h�[���̐���
			case oscenario::OBJ_CROW_DOLL:
				AddCrowDoll(ovalue);
				continue;
			// ���o�[�̐���
			case oscenario::OBJ_LEVER:
				AddLever(ovalue);
				continue;
			// �����̐���
			case oscenario::OBJ_CRYSTAL:
				AddCrystal(ovalue);
				continue;
			// �u���b�N�̐���
			case oscenario::OBJ_BLOCK:
				AddBlock(ovalue);
				continue;
			default:
#ifdef _DEBUG
				OutputDebugString("error�F�I�u�W�F�N�g�̐����Ɏ��s���܂����@Scenatio->AddObjects�ő��݂��Ȃ��N���X�̒l���o�^����Ă��܂�\n");
#endif
			}
		}
		return false;
	}


	bool Scenario::IsLoad(const std::string key) {
		auto it = _scenarios.find(key);
		if (it != _scenarios.end()) return false;	// �o�^����Ă���ꍇ�͂��̂܂ܐ����Ɉڂ�

		// �Y������V�i���I�̓ǂݍ��݂��s��
		std::vector<ObjectValue> _ovalue;
		if (key == stage::STAGE_0) _ovalue = Loads::LoadScenarioS();
		if (key == stage::STAGE_1) _ovalue = Loads::LoadScenario1();
		if (key == stage::STAGE_2) _ovalue = Loads::LoadScenario2();
		if (key == stage::STAGE_2_1) _ovalue = Loads::LoadScenario2_1();
		if (key == stage::STAGE_2_2) _ovalue = Loads::LoadScenario2_2();
		if (key == stage::STAGE_3) _ovalue = Loads::LoadScenarioB();
		LoadObjectData(key, _ovalue);	// �o�^
		return true;
	}

	void Scenario::AddPlayer() {
		if (_game.GetObjectServer()->IsPlayer() == true) return;
		ObjectValue ovalue(oscenario::OBJ_PLAYER, { 550, 905 });	// �X�e�[�WS�ɍ��킹���n�_�ɐ�������
		auto player = std::make_shared<Player>(_game.GetGame());
		player->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(player));
	}

	/*void Scenario::AddPlayer(std::shared_ptr<ObjectBase> obj, ObjectValue ovalue) {
		auto player = std::dynamic_pointer_cast<Player>(obj);
		player->
	}*/

	void Scenario::AddEnemy(ObjectValue ovalue) {
		auto enemy = std::make_shared<SoldierDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
		
	}

	void Scenario::AddSoldierDoll(ObjectValue ovalue) {
		auto enemy = std::make_shared<SoldierDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddBigDoll(ObjectValue ovalue) {
		auto enemy = std::make_shared<BigDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddCrowDoll(ObjectValue ovalue) {

	}

	void Scenario::AddSoul(ObjectValue ovalue) {

	}

	void Scenario::AddLever(ObjectValue ovalue) {
		auto glever = std::make_shared<Lever>(_game.GetGame());
		auto posv = ovalue.Positions();
		glever->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(glever));
	}

	void Scenario::AddCrystal(ObjectValue ovalue) {

	}

	void Scenario::AddBlock(ObjectValue ovalue) {

	}

	void Scenario::ScenarioUpdate(std::string key) {
		// ���ݓo�^����Ă���M�~�b�N�̒l���擾����
		auto Gimmicks = _game.GetObjectServer()->GetGimmicks();
		auto scenario = _scenarios.find(key);
#ifdef _DEBUG
		if (scenario == _scenarios.end()) {
			OutputDebugString("Scenario->ScenarioUpdate(std::string key)�F�o�^����Ă��Ȃ��L�[���n����Ă��܂��B�C�����Ă��������B\n");
			return;
		}
#endif
		std::vector<std::shared_ptr<GimmickBase>> gs;
		auto gsize = static_cast<int>(Gimmicks.size());
		int fix = 0;
		// �M�~�b�N�̐��l���C������
		for (auto gimmick = 0; gimmick < gsize; ++gimmick) {
			// if (gsize != static_cast<int>(Gimmicks.size())) fix = (gsize - static_cast<int>(Gimmicks.size()));
			if (Gimmicks[gimmick]->GimmickType() == gimmick::DOOR) continue;	// �h�A�̏ꍇ�͏������΂�
			Gimmicks[gimmick]->ObjValueUpdate();
			gs.emplace_back(std::move(Gimmicks[gimmick]));
		}
		Gimmicks.clear();	// ��ڂ��I�����̂Ń���������
		for (auto&& ovalue : scenario->second) {
			if (gs.empty())	break;	// �v�f����ɂȂ����ꍇ�͏������I������
			// �M�~�b�N�ɂ̂ݍX�V��������
			if (ovalue.ObjectType() != oscenario::type::GIMMICK) continue;
			ovalue = gs.front()->GetObjectValue();	// ���݂̏��ɏ㏑������
			gs.erase(gs.begin());	// ��[�̗v�f���폜����
		}
	}
}