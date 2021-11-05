/*****************************************************************//**
 * @file   EnemyBase.cpp
 * @brief  �G�̃X�[�p�[�N���X�i�I�u�W�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "SoulSkin.h"
#include "ChipNumber.h"

using std::string;

namespace inr {

  EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
    // �e��p�����[�^�̏�����
    _eType = EnemyType::SOLDIER_DOLL;
    _aState = ActionState::EMPTY;
    _divKey = std::make_pair("NULL", key::SOUND_NUM);
    _position = {};
    _moveVector = {};
    _actionX = 0;
    _patrolX = 0;
    _aInterval = 0;
    _aFrame = 0;
    _aCount = 0;
    _stay = 0;
    _direction = false;
    _changeGraph = true;
    _drawStop = false;
    _isAction = false;
    _soul = nullptr;

    Init();
  }

  EnemyBase::~EnemyBase() {
  }

  void EnemyBase::Init() {
  }

  void EnemyBase::Draw() {
    // �`����W�̎Z�o
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // �摜�̓ǂݍ��݂���ѕ`��
    int graph;
    GraphResearch(&graph);
    DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
    // �����蔻��̕`��
    DrawDebugBox(_searchBox);
    if (_mainCollision.GetCollisionFlag() == true) DrawDebugBox(_mainCollision);
#endif
  }

  void EnemyBase::AnimationCount() {
    // �����󂩂A�A�j���[�V�����ԍ����ő�̏ꍇ�͏������I��
    if (_soul == nullptr && IsAnimationMax() == true) return;
    ObjectBase::AnimationCount(); // �J�E���g�����Ăяo��
  }

  bool EnemyBase::SearchPlayer() {
    // ������ł͂Ȃ���
    if (_soul != nullptr) {
      // �v���C���[�N���X�̎Q�Ƃ��烁�C���̓����蔻����擾
      auto player = _game.GetObjectServer()->GetPlayer();
      const auto& pBox = player->GetMainCollision();
      // ���G�͈͂Ǝ��@�͏Փ˂��Ă��邩�H
      if (_searchBox.HitCheck(pBox)) {
        // ���@�͎����Ɠ��������ɂ��邩�H
        if (_direction == SearchPosition()) {
          return true; // ��������
        }
      }
    }
    return false;      // �����ł��Ȃ�����
  }

  bool EnemyBase::SearchPosition() {
    auto pl = _game.GetObjectServer()->GetPlayer();
    auto px = pl->GetPosition().GetX(); // ���@��x���W
    auto myx = _position.GetX();        // ���g��x���W
    // ���g��x���W���玩�@��x���W�������A���E�ǂ���ɂ��邩���Z�o����
    auto way = myx - px;
    if (way < 0) return false; // �E���ɋ���
    return true;               // �����ɋ���
  }

  void EnemyBase::PositionUpdate() {
  }

  void EnemyBase::EscapeOn() {
  }

  void EnemyBase::PatrolOn() {
    _searchBox.SetCollisionFlag() = true; // ���G�����̍ĊJ
    _changeGraph = true;          
    _aState = ActionState::PATROL;        // �����ԂɑJ��
  }

  void EnemyBase::AttackOn() {
  }

  void EnemyBase::ChangeState(ActionState nextState, std::string key) {
    // �e��p�����[�^�̐؂�ւ�
    _changeGraph = true;
    _aState = nextState;
    _divKey.first = key;
  }

  void EnemyBase::ChangeIdol(int stay) {
  }

  void EnemyBase::SetParameter(ObjectValue objValue) {
    _oValue = objValue;    // �I�u�W�F�N�g���̓o�^
    // �e��p�����[�^�̓o�^
    _position = _oValue.Positions()[0];
    _direction = _oValue.Direction();
    _mainCollision.Update(_position, _direction);
    _searchBox.Update(_position, _direction);
  }

  void EnemyBase::CollisionHit(const std::string cKey, Collision aCollision, bool direction) {
  }

  bool EnemyBase::IsEmpty() {
    if (_soul == nullptr) return true; // �����k
    return false;                      // �����k�ł͂Ȃ�
  }

  void EnemyBase::Death() {
    _soul->SetSpwan(_position); // ���g�̍��W�ɍ������̉�����
    _soul->OwnerNull();         // ���L�҂̏�����
    _soul.reset();              // ���̏��L���������
  }

  void EnemyBase::Action() {
    // �����𖞂����ĂȂ����ߏI��
    if (_aState == ActionState::WAKEUP || _isAction == true) return;
    // ���@�𔭌������ꍇ�̓A�N�V������ԂɈڍs
    if (SearchPlayer() == true) {
      _isAction = true;
    // ������̏ꍇ�̓A�N�V�����ړ��ʂ�0�ɂ���
    } else if (_soul == nullptr) _actionX = 0;
  }

  AABB EnemyBase::VitalPart(Collision& col, int vital) {
    // ���W���Z�o
    Vector2 vitalMin(0.0, col.GetMin().GetY());
    Vector2 vitalMax(0.0, col.GetMax().GetY());
    // �����ɉ�����x���W�ɏC����������
    switch (_direction) {
    case enemy::MOVE_LEFT:
      vitalMin.GetPX() = col.GetMax().GetX() - vital;
      vitalMax.GetPX() = col.GetMax().GetX();
      break;
    case enemy::MOVE_RIGHT:
      vitalMin.GetPX() = col.GetMin().GetX();
      vitalMax.GetPX() = col.GetMin().GetX() + vital;
      break;
    }
    // ������̐���
    return AABB(vitalMin, vitalMax, true);
  }

  AABB EnemyBase::DamageBox(int fix) {
    // �x�N�g���쐬
    auto damageMin(_mainCollision.GetMin());
    auto damageMax(_mainCollision.GetMax());
    // �����ɉ����Ĕ͈͏C��
    switch (_direction) {
    case enemy::MOVE_LEFT:
      damageMax.GetPX() -= fix;
      break;
    case enemy::MOVE_RIGHT:
      damageMin.GetPX() += fix;
      break;
    }
    // �����蔻��̏C��
    return AABB(damageMin, damageMax, _mainCollision.GetCollisionFlag());
  }

  bool EnemyBase::IsStandChip() {
    auto nowCol = NowCollision(_divKey.first); // ���݂̓����蔻����擾
    // �ڐG���Ă���}�b�v�`�b�v�𔻕ʂ���
    auto chipType = _game.GetMapChips()->IsStand(nowCol, _position, _gravity, &_lastChip);
    switch (chipType) {
      // �� or �ʏ�`�b�v�̏ꍇ
    case mapchip::IVY:
    case mapchip::NONE:
      return false; // ���ʖ���
    case mapchip::THORM:
      if (_soul != nullptr) Death(); // ��������ꍇ�͎��S�������Ăяo��
      return true;  // ���ʗL��
    default:
      return true;
    }
  }

  bool EnemyBase::PlaySe(const std::string key) {
    auto sound = SoundResearch(key); // �n���h���̎擾
    if (sound == -1) return false;   // �n���h���̓ǂݍ��݂Ɏ��s�����ꍇ�͏I������
    // �������Đ�����
    PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
    return true;
  }

  AABB EnemyBase::EmptyBox() {
    return NowCollision(_divKey.first);
  }

  bool EnemyBase::SoulPop() {
    if (_soul == nullptr) return false; // ���Ɏ���ł���
    Death();  // ���S
    return true;
  }
}