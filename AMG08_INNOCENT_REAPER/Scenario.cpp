/*****************************************************************//**
 * @file   Scenario.cpp
 * @brief  �V�i���I�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Scenario.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include <unordered_set>
#include <string>
#include <algorithm>
#include "ObjectValue.h"
#include "GimmickValue.h"
#include "Loads.h"
#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "BigDoll.h"
#include "CrowDoll.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "Door.h"
#include "Block.h"
#include "Crystal.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "ItemServer.h"
#include "Item.h"
#include "Particle_Image.h"
#include "TutorialImage.h"
#include "TutorialValue.h"
#include "TutorialServer.h"

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
	}

	void Scenario::LoadImageData(ImageMap imap) {
		for (auto image : imap) {
			// ���łɓo�^����Ă��邩�m�F�B�o�^����Ă��Ȃ��ꍇ�̂݁A�����𑱂���
			if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsResister(image.first) != true) continue;
			_game.GetModeServer()->GetModeMain()->GetItemImages()->AddImage(image.first, AddImage(image.second));
		}
	}

	void Scenario::ClearScenario() {
		_scenarios.clear();
	}

	bool Scenario::AddObjects(const std::string key) {
		IsLoad(key);				// �I�u�W�F�N�g�̓ǂݍ��݂��s�����H
		AddPlayer();				// �v���C���[�𐶐����邩�H
		AddTutorials(key);	// �`���[�g���A��UI�𐶐�����
		if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsLoad() == true) LoadImageData(Loads::LoadImages());

		auto it = _scenarios.find(key);
		// �o�^����Ă���I�u�W�F�N�g�̐���
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
			// �{�X��p�h�A�̐���
			case oscenario::OBJ_DOOR:
				AddDoor(ovalue);
				continue;
			// �A�C�e���̐���
			case oscenario::OBJ_ITEM:
				AddItem(ovalue);
				continue;
			default:
#ifdef _DEBUG
				OutputDebugString("error�F�I�u�W�F�N�g�̐����Ɏ��s���܂����@Scenatio->AddObjects�ő��݂��Ȃ��N���X�̒l���o�^����Ă��܂�\n");
#endif
				continue;
			}
		}
		return true;
	}

	bool Scenario::IsLoad(const std::string key) {
		auto it = _scenarios.find(key);
		if (it != _scenarios.end()) return false;	// �o�^����Ă���ꍇ�͂��̂܂ܐ����Ɉڂ�

		// �Y������V�i���I�̓ǂݍ��݂��s��
		std::vector<ObjectValue> _ovalue;
		if (key == stage::STAGE_0) _ovalue = Loads::LoadScenarioS();
		if (key == stage::STAGE_T) _ovalue = Loads::LoadScenarioS_1();
		if (key == stage::STAGE_1) _ovalue = Loads::LoadScenario1();
		if (key == stage::STAGE_2) _ovalue = Loads::LoadScenario2();
		if (key == stage::STAGE_2_1) _ovalue = Loads::LoadScenario2_1();
		if (key == stage::STAGE_2_2) _ovalue = Loads::LoadScenario2_2();
		if (key == stage::STAGE_3) _ovalue = Loads::LoadScenarioB();
		LoadObjectData(key, _ovalue);	// �o�^
		return true;
	}

	void Scenario::AddPlayer() {
		// ���Ɏ��@���o�^����Ă���ꍇ�͏������I������
		if (_game.GetObjectServer()->IsPlayer() == true) return;
#ifdef _DEBUG
		// ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 });
		ObjectValue ovalue(oscenario::OBJ_PLAYER, { 8900, 905 });
#endif
#ifndef _DEBUG
		ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 });	// �X�e�[�WS�ɍ��킹���n�_�ɐ�������
#endif
		auto player = std::make_shared<Player>(_game.GetGame());
		player->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(player));
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
		auto enemy = std::make_shared<CrowDoll>(_game.GetGame());
		enemy->SetParameter(ovalue);
		_game.GetObjectServer()->Add(std::move(enemy));
	}

	void Scenario::AddLever(ObjectValue ovalue) {
		auto glever = std::make_shared<Lever>(_game.GetGame());
		glever->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(glever));
	}

	void Scenario::AddCrystal(ObjectValue ovalue) {
		auto gcrystal = std::make_shared<Crystal>(_game.GetGame());
		gcrystal->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(gcrystal));
	}

	void Scenario::AddBlock(ObjectValue ovalue) {
		auto glever = std::make_shared<Block>(_game.GetGame());
		auto posv = ovalue.Positions();
		glever->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(glever));
	}

	void Scenario::AddDoor(ObjectValue ovalue) {
		auto gdoor = std::make_shared<Door>(_game.GetGame());
		gdoor->SetParameter(ovalue);
		_game.GetGimmickServer()->Add(std::move(gdoor));
	}

	std::unique_ptr<Particle_Image> Scenario::AddImage(ImageValue pvalue) {
		auto pimage = std::make_unique<Particle_Image>(_game.GetGame());
		pimage->SetImage(pvalue);
		return std::move(pimage);
	}

	void Scenario::AddItem(ObjectValue ovalue) {
		auto item = std::make_shared<Item>(_game.GetGame());
		item->SetParameter(ovalue);
		_game.GetModeServer()->GetModeMain()->GetItemServer()->Add(std::move(item));
	}

	void Scenario::ScenarioUpdate(std::string key) {
		// ���ݓo�^����Ă���M�~�b�N�̒l���擾����
		auto Gimmicks = _game.GetGimmickServer()->GetGimmicks();
		auto Items = _game.GetModeServer()->GetModeMain()->GetItemServer()->GetItems();
		// ���݂̃X�e�[�W�̃V�i���I
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

		for (auto ite : Items) ite->ObjValueUpdate();

		
		for (auto&& ovalue : scenario->second) {
			if (gs.empty() && Items.empty()) break;							// �v�f����ɂȂ����ꍇ�͏������I������
			if (ovalue.ObjectType() == oscenario::type::ITEM) {	// �A�C�e���̏ꍇ�͍X�V��������
				ovalue = Items.front()->GetObjectValue();					// �����擾
				Items.erase(Items.begin());
				continue;
			}
			if (gs.empty()) continue;
			// �M�~�b�N�ɂ̂ݍX�V��������
			if (ovalue.ObjectType() != oscenario::type::GIMMICK) continue;
			ovalue = gs.front()->GetObjectValue();	// ���݂̏��ɏ㏑������
			gs.erase(gs.begin());										// ��[�̗v�f���폜����
		}
	}

	void Scenario::BossBlock() {
		ObjectValue b_block = { oscenario::OBJ_BLOCK, {3840, 820}, false,  0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE} };
		AddBlock(b_block);
	}

	bool Scenario::AddTutorials(const std::string key) {
		if (key == stage::STAGE_0) {
			auto ite =  Loads::LoadTutorialF();
			for (auto&& it : ite) {
				auto timage = std::make_unique<TutorialImage>(_game.GetGame());
				timage->SetTutorialImage(it);
				_game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(timage));
			}
			return true;
		}
		else if (key == stage::STAGE_T) {
			auto ite = Loads::LoadTutorialS();
			for (auto&& it : ite) {
				auto timage = std::make_unique<TutorialImage>(_game.GetGame());
				timage->SetTutorialImage(it);
				_game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(timage));
			}
			return true;
		}
		else return false;
	}
}