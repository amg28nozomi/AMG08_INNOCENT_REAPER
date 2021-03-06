/*****************************************************************//**
 * @file   SoldierDoll.cpp
 * @brief  \W[h[iGl~[x[XΜTuNXj
 *
 * @author ιΨσC
 * @date   October 2021
 *********************************************************************/
#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "Player.h"
#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "EffectBase.h"
#include "EffectServer.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "SoulSkin.h"
#include <memory>

namespace {
  constexpr auto START_POS_X = 500;
  constexpr auto START_POS_Y = 400;
  // E³
  constexpr auto SOLDIER_W = 60;
  constexpr auto SOLDIER_H = 120;
  // ΚνσΤΜ½θ»θ(C³p)
  constexpr auto SF_HEIGHT = 20;
  constexpr auto SOLDIER_BOX_W = SOLDIER_W / 2;
  constexpr auto SOLDIER_BOX_H1 = SOLDIER_H / 2 - SF_HEIGHT;
  constexpr auto SOLDIER_BOX_H2 = SOLDIER_H / 2 + SF_HEIGHT;
  // ±ΒΝΝ 
  constexpr auto SOLDIER_VITAL = 20;
  // ½θ»θ
  constexpr auto COLLISION_W = 60;

  constexpr auto EMPTY_W = 250;
  constexpr auto EMPTY_H = 100;
  // υGΝΝ
  constexpr auto SEARCH_X = 200 + (SOLDIER_BOX_W * 2); // 260
  constexpr auto SEARCH_Y1 = SOLDIER_BOX_H1;
  constexpr auto SEARCH_Y2 = SOLDIER_BOX_H2;
  // ρΝΝ(ΕεVector)
  constexpr auto PATROL_VECTOR = 1.0;
  constexpr auto PATROL_MAX = 280;
  // UpΝΝ
  constexpr auto ATTACK_W1 = SOLDIER_BOX_W + 30;
  constexpr auto ATTACK_W2 = SOLDIER_W / 2;
  constexpr auto ATTACK_H = 30;

  constexpr auto FRAME = 30;
  // ACh[VΤ
  constexpr auto STAY_MAX = 60; // stay
  constexpr auto GIVE_STAY = 33 * 2;

  constexpr auto IDOL_FRAME = 33;
}

namespace inr {

  SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
    _type = ObjectBase::ObjectType::ENEMY;
    _eType = EnemyType::SOLDIER_DOLL;
    _aState = ActionState::EMPTY;
    _divKey = std::make_pair(enemy::SOLDIER_EMPTY, key::SOUND_NUM);
    _position = { 0, 0 };

    _aFrame = 0;
    _aCount = 0;
    _patrolX = 0;
    _aInterval = 0;
    _direction = false;
    _changeGraph = true;
    _drawStop = false;

    Init();
  }

  SoldierDoll::~SoldierDoll() {
  }

  void SoldierDoll::Init() {
    // CΜ½θ»θπϊ»
    _mainCollision = { _position, 30, 60, true };
    // υG{bNXΜo^
    _searchBox = { _position, SEARCH_X, SEARCH_X, 70, 60, true };
    // eν»θ{bNXΜi[i²―kEUj
    _collisions = {
      { enemy::SOLDIER_EMPTY, { _position, 60, 60, -25, 60 , true}}, // ²―kΜξρ
      { enemy::red::SOLDIER_ATTACK, {_position, ATTACK_W1, ATTACK_W2, ATTACK_H, ATTACK_H, true}},
    };

    _motionKey =
    {  { enemy::SOLDIER_EMPTY, {19 * 3, 0}},

      { enemy::red::SOLDIER_WAKEUP, {17 * 3, 0}},
      { enemy::red::SOLDIER_IDOL, {11 * 3, 0}},
      { enemy::red::SOLDIER_PATROL, {13 * 3, 13 * 3}},
      { enemy::red::SOLDIER_ATTACK, {11 * 3, 50}}, // SELθ

      { enemy::blue::SOLDIER_WAKEUP, {17 * 3, 0}},
      { enemy::blue::SOLDIER_IDOL, {11 * 3, 0}},
      { enemy::blue::SOLDIER_PATROL, {13 * 3, 13 * 3}},
      { enemy::blue::SOLDIER_ESCAPE, {13 * 3, 0}}
    };

    PatrolOn();
  }

  void SoldierDoll::Process() {
    ObjectBase::Process();
    _moveVector.GetPX() = 0;
    AnimationCount();
    Patrol();
    Action();
    Attack();
    Move();
    PositionUpdate();
  }

  void SoldierDoll::StateUpdate() {
    if (_soul == nullptr) return; // °ͺΘ’κΝπIΉ
    switch (_soul->SoulColor()) {
    case soul::RED:
      if (_aInterval) {
        --_aInterval;
      }
      return;
    case soul::BLUE:
      if (_aInterval) {
        --_aInterval;
      }
      return;
    }
  }

  void SoldierDoll::Action() {
    if (_aState == ActionState::WAKEUP || _isAction == true) return;
    // vC[π­©Ε«ι©
    if (SearchPlayer() == true) {
      _isAction = true;
      ChangeIdol(IDOL_FRAME);

      auto eyeLight = std::make_unique<EffectBase>(_game.GetGame(), effect::soldier::OMEN, _position, effect::soldier::OMEN_NUMS * 3, _direction);
      _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eyeLight), effect::type::FORMER);
      switch (_soul->SoulColor()) {
      case soul::RED:
        PlaySe(enemy::soldier::ATTACK_VOICE);
        break;
      case soul::BLUE:
        PlaySe(enemy::soldier::ESCAPE_VOICE);
        break;
      }
    }
    // ­©Ε«Θ©Α½κΝΪ?πs€
    if (_soul == nullptr) _actionX = 0;
  }

  void SoldierDoll::Patrol() {
    if (_soul == nullptr) return; // °ͺσΜκΝπsνΘ’
    if (_stay != 0) {
      --_stay; // ?@[VΝJE^πΈη΅Δπ²―ι
      if (_stay == 0) { 
        _mainCollision.SetCollisionFlag() = true;
      }
      return;
    }

    switch (_aState) {
    case ActionState::IDOL:
      if (_mainCollision.GetCollisionFlag() != true) _mainCollision.SetCollisionFlag() = true;  
      if (_isAction == true) {
        switch (_soul->SoulColor()) {
          case soul::RED:
            AttackOn();
            _isAction = false;
            return;
          case soul::BLUE:
            EscapeOn();
            _isAction = false;
            return;
        }
      }
      PatrolOn();
      _searchBox.SetCollisionFlag() = true;
      return;
    case ActionState::PATROL:
      // ΆΪ?
      if (_patrolX < 0) {
        _moveVector.GetPX() = PATROL_VECTOR;
        _patrolX += PATROL_VECTOR;
        return;
      // EΪ?
      } else if (0 < _patrolX) {
        _moveVector.GetPX() = -PATROL_VECTOR;
        _patrolX -= PATROL_VECTOR;
        return;
      }
      // Ϊ?Κͺ0ΜκΝ?@σΤΙJΪ·ι
      _aState =  ActionState::IDOL;
      switch (_soul->SoulColor()) {
      case soul::RED:
        _divKey.first = enemy::red::SOLDIER_IDOL;
        break;
      case soul::BLUE:
        _divKey.first = enemy::blue::SOLDIER_IDOL;
        break;
      }
      _stay = STAY_MAX;
      return;
    // N«γͺθ[V
    case ActionState::WAKEUP:
      // N«γͺθAj[VΜΔΆͺIνΑ½ΘηAρσΤΙJΪ·ι
      if (AnimationCountMax() == true) {
        ChangeIdol(GIVE_STAY);
#ifdef _DEBUG
        _searchBox.SetDrawFlag() = true;
#endif
      }
      return;
    }
  }

  void SoldierDoll::Move() {
    // Ϊ?xNgΙΆΔAό«πΟX
    // Ϊ?ΚΙΆΔό«πΟX
    if (_actionX < 0) { 
      _direction = enemy::MOVE_LEFT; }
    else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;
    Escape();
  }

  void SoldierDoll::Escape() {
    switch (_aState) {
    case ActionState::PATROL:
      if (0 < _patrolX) _direction = enemy::MOVE_LEFT;
      else if (_patrolX < 0) _direction = enemy::MOVE_RIGHT;
      return;

    case ActionState::ESCAPE:
      if (_aCount == 0 && _stand == true) PlaySe(enemy::soldier::ESCAPE_SE);
      if (_actionX < 0) {
        _direction = enemy::MOVE_LEFT;
      }
      else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;

      if (_actionX != 0) {
        switch (_direction) {
        case enemy::MOVE_LEFT:
          _actionX += enemy::ESCAPE_VECTOR / FRAME;
          _moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
          break;
        case enemy::MOVE_RIGHT:
          _actionX -= enemy::ESCAPE_VECTOR / FRAME;
          _moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
          break;
        }
        if (_actionX == 0) { 
          ChangeIdol(IDOL_FRAME);
        }
      }
      return;
    case ActionState::ATTACK:
      if (_aCount == 0) PlaySe(enemy::soldier::ATTACK_SE);
      if (_actionX < 0) {
        _direction = enemy::MOVE_RIGHT;
      }
      else if (0 < _actionX) _direction = enemy::MOVE_LEFT;

      if (_actionX != 0) {
        double mv = 0;
        switch (_direction) {
        case enemy::MOVE_LEFT:
          _actionX -= enemy::ATTACK_VECTOR / FRAME;
          mv = -enemy::ATTACK_VECTOR / FRAME;
          if (_actionX < 0) _actionX = 0;
          break;
        case enemy::MOVE_RIGHT:
          _actionX += enemy::ATTACK_VECTOR / FRAME;
          mv = enemy::ATTACK_VECTOR / FRAME;
          if (0 < _actionX) _actionX = 0;
          break;
        }
        _moveVector.GetPX() = mv;
        if (_actionX == 0) { 
          ChangeIdol(IDOL_FRAME);
        }
      }
      return;
    default:
      return;
    }
  }

  void SoldierDoll::PatrolOn() {
    if (_soul == nullptr) return;
    EnemyBase::PatrolOn();
    StopSound();
    // °ΜFΙΆΔL[πΨθΦ¦
    (_soul->SoulColor() == soul::BLUE) ?
      _divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

    if (_actionX == 0) {
      if(_direction) _patrolX = -PATROL_MAX;
      else _patrolX = PATROL_MAX;
    }
  }

  void SoldierDoll::AttackOn() {
    if (_aState != ActionState::ATTACK) {
      ChangeState(ActionState::ATTACK, enemy::red::SOLDIER_ATTACK);
      (_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
      _searchBox.SetCollisionFlag() = false; // ANVΙΛό΅½ηκIΙυGπΨι
    }
  }

  void SoldierDoll::EscapeOn() {
    // ¦σΤΕΝΘ’κΜέA
    if (_aState != ActionState::ESCAPE) {
      ChangeState(ActionState::ESCAPE, enemy::blue::SOLDIER_ESCAPE);
      (_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
      _searchBox.SetCollisionFlag() = false; // ANVΙΛό΅½ηκIΙυGπΨι
    }
  }

  void SoldierDoll::PositionUpdate() {
    // Ϊ?xNgYΙΑ¬xπγό
    _moveVector.GetPY() = _gravity;
    // }bv`bvΙίθρΕ’ικΝΐWπC³
    if (_soul != nullptr) {
      int i = 0;
    }
    _game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);
    GimmickCheck(_moveVector);
    _position = _position + _moveVector; // ΚuΐWπXV

    _mainCollision.Update(_position, _direction);
    _searchBox.Update(_position, _direction);

    if (_aState == ActionState::ATTACK) {
      auto it = _collisions.find(_divKey.first);
      it->second.Update(_position, _direction);
#ifdef _DEBUG
      if(it->second.SetDrawFlag() == false) it->second.SetDrawFlag() = true;
#endif
    }
    auto col = _collisions.find(enemy::SOLDIER_EMPTY);
    col->second.Update(_position, _direction);

    if (_soul == nullptr && IsAnimationMax() == true) {
      col->second.SetCollisionFlag() = true;
#ifdef _DEBUG
      _searchBox.SetDrawFlag() = false;
#endif
    }
  }

  void SoldierDoll::Attack() {
    if (_soul == nullptr || _aState == ActionState::WAKEUP) return; // °ͺΘ’κΝπXLbv
    auto&& player = _game.GetObjectServer()->GetPlayer();
    auto playerBox = player->GetMainCollision(); // vC[Μ½θ»θπζΎ
    // M~bNio[jΖΥΛ΅½©H
    if (_aState == ActionState::ATTACK) {
      auto gs = _game.GetGimmickServer()->GetGimmicks();
      for (auto gg : gs) {
        if (gg->GimmickType() != gimmick::LEVER) continue; // o[ΕΝΘ’κΝXLbv
        if (_mainCollision.HitCheck(gg->GetMainCollision())) {
          std::dynamic_pointer_cast<Lever>(gg)->OpenDoor();
        }
      }
    }
    // ©gΜ½θ»θΖΪG»θπs€
    auto dmb = DamageBox(10);
    if (dmb.HitCheck(playerBox)) {
      player->Damage(SearchPosition());
      return;
    }
    // UσΤΜκΝΗΑΕπs€
    if (_aState == ActionState::ATTACK) {
      auto box = _collisions.find(_divKey.first);
      // ΪG΅Δ’ικΝ©@ΜΥΛπΔΡo·
      if (box->second.HitCheck(playerBox)) {
        // isϋόΖΝ―ϋόΙΞΫπςΞ·
        player->Damage(SearchPosition());
      }
    }
  }

  void SoldierDoll::ChangeIdol(int stay) {
    _stay = stay;
    _searchBox.SetCollisionFlag() = false; // ½θ»θπΨι
    std::string nextKey = "";
    switch (_soul->SoulColor()) {
    case soul::RED:
      nextKey = enemy::red::SOLDIER_IDOL;
      break;
    case soul::BLUE:
      nextKey = enemy::blue::SOLDIER_IDOL;
      break;
    }
    ChangeState(ActionState::IDOL, nextKey);
  }

  void SoldierDoll::CollisionHit(const std::string cKey, Collision aCollision, bool direction) {
    // »έΜ}ͺ ιΐWπZo
    auto vitalPart = VitalPart(_mainCollision, SOLDIER_VITAL);
    auto player = _game.GetObjectServer()->GetPlayer();
    // °ΝDνκι©H
    if (cKey == PKEY_ROB) {
      if (_soul != nullptr) {
        if (_direction == direction && vitalPart.HitCheck(aCollision)) {
          // °πDνκι
          ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
          StopSound();
          PlaySe(enemy::soldier::DOWN);
          _searchBox.SetCollisionFlag() = false; // κIΙυG»θπΨι
          _mainCollision.SetCollisionFlag() = false;
          _soul->SetSpwan(_position); // ©gΜSΐWΙΐΜ»³Ήι
          _stay = 0;
          _isAction = false;
          auto hitEff = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy:: S_HIT, _position, 30);
          _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hitEff), effect::type::FORMER);
          // ©@ͺΫL·ι°ͺγΐΙB΅Δ’ικΝL πθϊ·
          if (player->IsSoulMax()) {
            _soul->OwnerNull();
            _soul.reset(); // L πθϊ·
            return;
          }
          player->SoulCatch(std::move(_soul)); // °ΜL πvC[Ιχn
          return;
        }
      }
    }
    // °π^¦ηκι©H
    if (cKey == PKEY_GIVE) {
      // vC[Ν°π΅Δ’ι©H
      if(player->HaveSoul()){
        // °ͺσΜκΙ{bNXͺΪG΅½η
        if (_soul == nullptr) {
          // ΪGΜ»θΝAABBΕs€iD€ANVΖΝα’Aό«Ιζι§ΐΘ΅j
          if (NowCollision(_divKey.first).HitCheck(aCollision)) {
            // vC[πζΎ
            auto player = _game.GetObjectServer()->GetPlayer();
            _soul = player->GiveSoul(); // vC\©ηΞΫΜ°πσ―ζι
            _soul->Inactive();          // °πρ«σΤΙ·ι
            PlaySe(key::SOUND_PLAYER_GIVE_TRUE);
            _searchBox.SetCollisionFlag() = false;
            switch (_soul->SoulColor()) {
            case soul::RED:
              ChangeState(ActionState::WAKEUP, enemy::red::SOLDIER_WAKEUP);
              return;
            case soul::BLUE:
              ChangeState(ActionState::WAKEUP, enemy::blue::SOLDIER_WAKEUP);
              return;
            }
            return;
          }
        }
      }
    }
  }

  void SoldierDoll::SetParameter(ObjectValue objValue) {
    EnemyBase::SetParameter(objValue);
    if (_oValue.SoulType() == 0) { // °ͺσΜκΝ²―kΙΘι
      auto embox = _collisions.find(enemy::SOLDIER_EMPTY);
      embox->second.Update(_position, _direction);
      ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
      _aCount = AnimationCountMax(); // JE^π}bNXΙ·ι
      _searchBox.SetCollisionFlag() = false; // κIΙυG»θπΨι
      _mainCollision.SetCollisionFlag() = false;
      _changeGraph = false;
      return; // π²―ι
    }
    auto soul_n = std::make_shared<SoulSkin>(_game.GetGame());
    switch (_oValue.SoulType()) {
    case 1:
      soul_n->SetParameter(_oValue.SoulType(), 7);
      ChangeState(ActionState::PATROL, enemy::red::SOLDIER_PATROL);
      break;
    case 2:
      soul_n->SetParameter(_oValue.SoulType(), 7);
      ChangeState(ActionState::PATROL, enemy::blue::SOLDIER_PATROL);
      break;
    default:
#ifdef _DEBUG
      OutputDebugString("errorFEnemyBase->SetParameter@ObjectValueΜ_soulTypeΜlͺs³Ε·\n");
#endif
      break;
    }
    _soul = soul_n;
    _game.GetObjectServer()->Add(std::move(soul_n));
  }

  void SoldierDoll::Death() {
    ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
    EnemyBase::Death();
    PlaySe(enemy::soldier::DOWN);
  }

  AABB SoldierDoll::NowCollision(std::string key) {
    if(_soul != nullptr) return _mainCollision;
    auto it = _collisions.find(enemy::SOLDIER_EMPTY);
    // »έΜANVσΤΝ{bNXπC³·ιKvͺ ι©H
    return it->second;
  }

  bool SoldierDoll::StopSound() {
    auto attack = se::SoundServer::GetSound(enemy::soldier::ATTACK_SE);
    auto escape = se::SoundServer::GetSound(enemy::soldier::ESCAPE_SE);
    if (CheckSoundMem(attack) == 1) StopSoundMem(attack);
    if (CheckSoundMem(escape) == 1) StopSoundMem(escape);
    return true;
  }
}