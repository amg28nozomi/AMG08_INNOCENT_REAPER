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
    Init();
  }

  Scenario::~Scenario() {
    ClearScenario();
  }

  void Scenario::Init() {
    _scenarios.clear();
  }

  void Scenario::LoadObjectData(std::string key , std::vector<ObjectValue> oValues) {
    auto it = _scenarios.find(key);
    if (it == _scenarios.end()) {
      _scenarios.emplace(key, oValues);
    }
  }

  void Scenario::LoadImageData(ImageMap iMap) {
    for (auto image : iMap) {
      // ���łɓo�^����Ă��邩�m�F�B�o�^����Ă��Ȃ��ꍇ�̂݁A�����𑱂���
      if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsResister(image.first) != true) continue;
      _game.GetModeServer()->GetModeMain()->GetItemImages()->AddImage(image.first, AddImage(image.second));
    }
  }

  void Scenario::ClearScenario() {
    _scenarios.clear();
  }

  bool Scenario::AddObjects(const std::string key) {
    IsLoad(key);       // �I�u�W�F�N�g�̓ǂݍ��݂��s�����H
    AddPlayer();       // �v���C���[�𐶐����邩�H
    AddTutorials(key); // �`���[�g���A��UI�𐶐�����
    if (_game.GetModeServer()->GetModeMain()->GetItemImages()->IsLoad() == true) LoadImageData(Loads::LoadImages());

    auto it = _scenarios.find(key);
    // �o�^����Ă���I�u�W�F�N�g�̐���
    for (auto ovalue : it->second) {
      auto classType = ovalue.ClassName(); // ��������I�u�W�F�N�g�͉��Ȃ̂��H
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
    if (it != _scenarios.end()) return false; // �o�^����Ă���ꍇ�͂��̂܂ܐ����Ɉڂ�

    // �Y������V�i���I�̓ǂݍ��݂��s��
    std::vector<ObjectValue> objValues;
    if (key == stage::STAGE_0) objValues = Loads::LoadScenarioS();
    if (key == stage::STAGE_T) objValues = Loads::LoadScenarioS_1();
    if (key == stage::STAGE_1) objValues = Loads::LoadScenario1();
    if (key == stage::STAGE_2) objValues = Loads::LoadScenario2();
    if (key == stage::STAGE_2_1) objValues = Loads::LoadScenario2_1();
    if (key == stage::STAGE_2_2) objValues = Loads::LoadScenario2_2();
    if (key == stage::STAGE_3) objValues = Loads::LoadScenarioB();
    LoadObjectData(key, objValues); // �o�^
    return true;
  }

  void Scenario::AddPlayer() {
    // ���Ɏ��@���o�^����Ă���ꍇ�͏������I������
    if (_game.GetObjectServer()->IsPlayer() == true) return;
#ifdef _DEBUG
    ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 });
#endif
#ifndef _DEBUG
    ObjectValue ovalue(oscenario::OBJ_PLAYER, { 560, 905 }); // �X�e�[�WS�ɍ��킹���n�_�ɐ�������
#endif
    auto player = std::make_shared<Player>(_game.GetGame());
    player->SetParameter(ovalue);
    _game.GetObjectServer()->Add(std::move(player));
  }

  void Scenario::AddSoldierDoll(ObjectValue oValue) {
    auto enemy = std::make_shared<SoldierDoll>(_game.GetGame());
    enemy->SetParameter(oValue);
    _game.GetObjectServer()->Add(std::move(enemy));
  }

  void Scenario::AddBigDoll(ObjectValue oValue) {
    auto enemy = std::make_shared<BigDoll>(_game.GetGame());
    enemy->SetParameter(oValue);
    _game.GetObjectServer()->Add(std::move(enemy));
  }

  void Scenario::AddCrowDoll(ObjectValue oValue) {
    auto enemy = std::make_shared<CrowDoll>(_game.GetGame());
    enemy->SetParameter(oValue);
    _game.GetObjectServer()->Add(std::move(enemy));
  }

  void Scenario::AddLever(ObjectValue oValue) {
    auto gLever = std::make_shared<Lever>(_game.GetGame());
    gLever->SetParameter(oValue);
    _game.GetGimmickServer()->Add(std::move(gLever));
  }

  void Scenario::AddCrystal(ObjectValue oValue) {
    auto gCrystal = std::make_shared<Crystal>(_game.GetGame());
    gCrystal->SetParameter(oValue);
    _game.GetGimmickServer()->Add(std::move(gCrystal));
  }

  void Scenario::AddBlock(ObjectValue oValue) {
    auto gBlock = std::make_shared<Block>(_game.GetGame());
    auto posV = oValue.Positions();
    gBlock->SetParameter(oValue);
    _game.GetGimmickServer()->Add(std::move(gBlock));
  }

  void Scenario::AddDoor(ObjectValue oValue) {
    auto gDoor = std::make_shared<Door>(_game.GetGame());
    gDoor->SetParameter(oValue);
    _game.GetGimmickServer()->Add(std::move(gDoor));
  }

  std::unique_ptr<Particle_Image> Scenario::AddImage(ImageValue pValue) {
    auto pImage = std::make_unique<Particle_Image>(_game.GetGame());
    pImage->SetImage(pValue);
    return std::move(pImage);
  }

  void Scenario::AddItem(ObjectValue oValue) {
    auto item = std::make_shared<Item>(_game.GetGame());
    item->SetParameter(oValue);
    _game.GetModeServer()->GetModeMain()->GetItemServer()->Add(std::move(item));
  }

  void Scenario::ScenarioUpdate(std::string key) {
    // ���ݓo�^����Ă���M�~�b�N�̒l���擾����
    auto gimmicks = _game.GetGimmickServer()->GetGimmicks();
    auto items = _game.GetModeServer()->GetModeMain()->GetItemServer()->GetItems();
    // ���݂̃X�e�[�W�̃V�i���I
    auto scenario = _scenarios.find(key);
#ifdef _DEBUG
    if (scenario == _scenarios.end()) {
      OutputDebugString("Scenario->ScenarioUpdate(std::string key)�F�o�^����Ă��Ȃ��L�[���n����Ă��܂��B�C�����Ă��������B\n");
      return;
    }
#endif
    std::vector<std::shared_ptr<GimmickBase>> gs;
    auto gSize = static_cast<int>(gimmicks.size());
    int fix = 0;
    // �M�~�b�N�̐��l���C������
    for (auto gimmick = 0; gimmick < gSize; ++gimmick) {
      if (gimmicks[gimmick]->GimmickType() == gimmick::DOOR) continue; // �h�A�̏ꍇ�͏������΂�
      gimmicks[gimmick]->ObjValueUpdate();
      gs.emplace_back(std::move(gimmicks[gimmick]));
    }
    gimmicks.clear();

    for (auto ite : items) ite->ObjValueUpdate();

    for (auto&& oValue : scenario->second) {
      if (gs.empty() && items.empty()) break;             // �v�f����ɂȂ����ꍇ�͏������I������
      if (oValue.ObjectType() == oscenario::type::ITEM) { // �A�C�e���̏ꍇ�͍X�V��������
        oValue = items.front()->GetObjectValue();         // �����擾
        items.erase(items.begin());
        continue;
      }
      if (gs.empty()) continue;
      // �M�~�b�N�ɂ̂ݍX�V��������
      if (oValue.ObjectType() != oscenario::type::GIMMICK) continue;
      oValue = gs.front()->GetObjectValue(); // ���݂̏��ɏ㏑������
      gs.erase(gs.begin());                  // ��[�̗v�f���폜����
    }
  }

  void Scenario::BossBlock() {
    ObjectValue oValue = { oscenario::OBJ_BLOCK, {3840, 820}, false,  0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE} };
    auto gBlock = std::make_shared<Block>(_game.GetGame());
    auto posV = oValue.Positions();
    gBlock->SetParameter(oValue);
    gBlock->SetInvalid();
    _game.GetGimmickServer()->Add(std::move(gBlock));
  }

  bool Scenario::AddTutorials(const std::string key) {
    if (key == stage::STAGE_0) {
      auto ite =  Loads::LoadTutorialF();
      for (auto&& it : ite) {
        auto tImage = std::make_unique<TutorialImage>(_game.GetGame());
        tImage->SetTutorialImage(it);
        _game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(tImage));
      }
      return true;
    }
    else if (key == stage::STAGE_T) {
      auto ite = Loads::LoadTutorialS();
      for (auto&& it : ite) {
        auto tImage = std::make_unique<TutorialImage>(_game.GetGame());
        tImage->SetTutorialImage(it);
        _game.GetModeServer()->GetModeMain()->GetTutorialServer()->Add(std::move(tImage));
      }
      return true;
    }
    else return false;
  }
}