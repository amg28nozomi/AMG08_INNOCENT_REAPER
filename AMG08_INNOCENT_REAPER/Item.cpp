/*****************************************************************//**
 * @file   Item.cpp
 * @brief  �A�C�e���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Item.h"
#include "Game.h"
#include "MapChips.h"
#include "Particle_Image.h"
#include "ObjectServer.h"
#include "Player.h"
#include "ObjectValue.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "ImageServer.h"
#include "SoundServer.h"

namespace {
  constexpr auto ITEM_MAX = 28 * 2; // �ő�t���[����
}

namespace inr {

  Item::Item(Game& game) : _game(game), _oValue() {
    // �e�평����
    _gkey = "";
    _position = { 0, 0 };
    _col = { _position, 0, 0, true };
    _count = 0;
    _maxFrame = ITEM_MAX;
    _del = false;
    _isGet = false;
  }

  void Item::Init() {
  }

  void Item::Process() {
    if (_isGet == true) return;       // ���ɓ��肳��Ă���ꍇ�͏������s��Ȃ�
    // �J�E���^�̐���
    if (_count < _maxFrame) ++_count; // �ő�t���[�������̏ꍇ�̓J�E���^�����Z
    else _count = 0;                  // ����ɓ��B�����ꍇ�͏�����

    auto pBox = _game.GetObjectServer()->GetPlayer()->GetMainCollision(); // ���@�̓����蔻����擾
    if (_col.HitCheck(pBox) == true) {
      // �ڐG��Ԃ��A�{�^�����͂��������ꍇ��message��\������
      if ((_game.GetTrgKey() == PAD_INPUT_4) == true) {
        _game.GetModeServer()->GetModeMain()->GetItemImages()->ImageChange(_messageNo);

        auto sound = se::SoundServer::GetSound(system::SOUND_INTERACTION);
        PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUND_INTERACTION));
        _isGet = true; // ���肵��
      }
    }
  }

  void Item::Draw() {
    if (_isGet == true) return; // ���肳��Ă���ꍇ�͕`�悵�Ȃ�
    // �`����W�̎Z�o
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // �`��摜�̎Z�o
    auto allNum = graph::ResourceServer::GetAllNum(_gkey);
    auto interval = _maxFrame / allNum;   // �P�\���Ԃ̊���o��
    auto no = _count / interval % allNum; // �`�悷��摜�̎Z�o
    auto gh = graph::ResourceServer::GetHandles(_gkey, no);
    DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
  }

  void Item::SetParameter(ObjectValue ovalue) {
    _oValue = ovalue; // ���o�^
    // �e�평����
    _gkey = item::ITEM;
    _isGet = ovalue.GimmickFlag() == TRUE;
    _messageNo = ovalue.GimmickType();
    _position = ovalue.Positions().front();
    _col = { _position, 50, 50, true };
  }

  void Item::ObjValueUpdate() {
    if (_oValue.GimmickFlag() == TRUE) return;    // ���Ƀt���O�I���̏ꍇ�͍X�V���s��Ȃ�
    if (_isGet == true) _oValue.FlagUpdate(TRUE); // �t���O��؂�ւ���
  }
}