/*****************************************************************//**
 * @file   Lever.cpp
 * @brief  ���o�[�N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Lever.h"
#include "Game.h"
#include "GimmickServer.h"
#include "SoundServer.h"
#include "Door.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "GimmickValue.h"

namespace inr {

  Lever::Lever(Game& game) : GimmickBase(game) {
    // ������
    _gType = GimmickType::LEVER;
    _door.reset();
    _divKey = { gimmick::lever::KEY_LEVER, "" }; // �L�[�̐ݒ�
    // �A�j���[�V�����̐ݒ�
    _motionKey = { 
      { _divKey.first, {20, 50}},
      { gimmick::lever::KEY_LEVER_BOSS, {30, 50}}
    };
  }

  void Lever::Process() {
    if (IsAnimationMax()) return; // �A�j���[�V�����ԍ����ő�̏ꍇ�͏������I��
    // �����J���Ă���ꍇ�̓A�j���[�V�������s��
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) ++_aCount;
      return;
    }
    else if (_door->IsSwitch()) ++_aCount;
  }

  void Lever::Draw() {
    // �`����W�̎Z�o
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // �O���t�B�b�N�n���h���̎擾
    int graph;
    GraphResearch(&graph);
    DrawRotaGraph(x, y, 1.0, 0, graph, true); // �`��
#ifdef _DEBUG
    DrawDebugBox(_mainCollision); // �����蔻��̕`��
#endif
  }

  void Lever::OpenDoor() {
    // ���ɔ����J���Ă���ꍇ�͏����𒆒f
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) return;
    }
    else if (_door->IsSwitch() == gimmick::ON ) return; // �h�A���J���Ă���ꍇ�͏����𔲂���
    // ���SE���擾���čĐ����s��
    auto sh = SoundResearch(gimmick::lever::KEY_LEVER);
    PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) { 
      // �{�X���̏ꍇ�͐�p�t���O���N������
      // �{�X�X�e�[�W�ւƑ��������������
      _game.GetModeServer()->GetModeMain()->OpenBossStage();
    }
    else _door->SwitchOn(); // �X�C�b�`�I��
#ifdef _DEBUG
    _mainCollision.SetDrawFlag() = false; // �����蔻��̕`����I��
#endif
  }

  void Lever::SetParameter(ObjectValue objValue) {
    _oValue = objValue; // �o�^
    // �e��p�����[�^�̎Z�o
    _position = _oValue.Positions()[0];
    _mainCollision = {  _position, 30, 30, 50, 70, true  };
    // �{�X�p���o�[�̏ꍇ�͐�p�������s��
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      _divKey.first = gimmick::lever::KEY_LEVER_BOSS; // �O���t�B�b�N�؂�ւ�
      // ���ɔ����J���Ă���ꍇ
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) {
        _aCount = AnimationCountMax();
#ifdef _DEBUG
        _mainCollision.SetDrawFlag() = false;
#endif
      }
      return;
    }
    // �h�A�̐���
    auto gDoor = std::make_shared<Door>(_game.GetGame());
    // �摜�̃L�[�̎擾
    std::string gh;
    switch (_oValue.GimmickType()) {
    case gimmick::door::D_LEVER:
      gh = gimmick::door::KEY_DOOR_LEVER;
      break;
    case gimmick::door::D_RED:
      gh = gimmick::door::D_RED;
      break;
    case gimmick::door::D_BLUE:
      gh = gimmick::door::D_BLUE;
      break;
    default:
      return;
    }
    // ���ɔ����J����Ă���ꍇ�̓A�j���[�V�����ԍ��Ɠ����蔻����C��
    if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
      _aCount = AnimationCountMax();
#ifdef _DEBUG
      _mainCollision.SetDrawFlag() = false;
#endif
    }
    // ����o�^����
    gDoor->SetParameter(_oValue.Positions()[1], gh, _oValue.GimmickFlag());
    _door = gDoor; // �o�^
    _game.GetGimmickServer()->Add(gDoor); // �M�~�b�N�T�[�o�ɓo�^
  }

  void Lever::ObjValueUpdate() {
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) return; // �������s��Ȃ�
    auto flag = oscenario::gimmick::FLAG_FALSE;
    // �t���O�؂�ւ�
    if (_door->IsSwitch() == gimmick::ON) flag = oscenario::gimmick::FLAG_TRUE;
    _oValue.FlagUpdate(flag); // �o�^���̍X�V
  }
}