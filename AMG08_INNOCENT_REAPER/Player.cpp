/*****************************************************************//**
 * @file   Player.cpp
 * @brief  自機クラス（オブジェクトベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Player.h"
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "EnemyBase.h"
#include "SoundServer.h"
#include "SoulSkin.h"
#include "MapChips.h"
#include "ChipNumber.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "GimmickBase.h"
#include "Crystal.h"
#include "EffectBase.h"
#include "TrackingEffect.h"
#include "EffectServer.h"
#include <DxLib.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace {
#ifdef _RELEASE
  constexpr auto START_POSITION_X = inr::WINDOW_H / 2;
  constexpr auto START_POSITION_Y = inr::WINDOW_W / 2;
#endif
#ifdef _DEBUG
  constexpr auto START_POSITION_X = 200;
  constexpr auto START_POSITION_Y = 1900;
#endif
  constexpr auto MAX_SPPED = 6;
  // ボックス座標修正用
  constexpr auto BF_HEIGHT_MIN = 10;
  constexpr auto BF_HEIGHT_MAX = 20;
  constexpr auto BF_WIDTH = 10;
  // アクションのフレーム数
  constexpr auto ACTION_MAX = 3;
  // SEのフレーム数
  constexpr auto SE_RUN1 = 37;     // 移動SEのフレーム数 
  // 奪うアクションの当たり判定
  constexpr auto ROB_WIDTH1 = 100;
  constexpr auto ROB_WIDTH2 = -25;
  constexpr auto ROB_HEIGHT1 = 10; // 0
  constexpr auto ROB_HEIGHT2 = 50; // 40
  // 奪うアクションの判定フレーム数
  constexpr auto JUDGE_MIN = -1;
  constexpr auto ROB_JUDGEMENT = 8 * 4;
  // 与えるアクションの当たり判定
  constexpr auto GIVE_WIDTH1 = 90;
  constexpr auto GIVE_WIDTH2 = -10;
  constexpr auto GIVE_HEIGHT1 = -15;
  constexpr auto GIVE_HEIGHT2 = 55;
  // 与えるアクションの判定フレーム数
  constexpr auto GIVE_JUDGEMENT = 10 * 4;
  // ジャンプアクション
  constexpr auto JUMP_VECTOR = 1; // ジャンプの移動ベクトル
  constexpr auto JUMP_MIN = 5.5;
  constexpr auto JUMP_MAX = 11;   // 15
  constexpr auto JUMP_Y = 5;

  constexpr auto JUMP_EFFECT_Y = 30;
  // ダッシュアクション関連
  constexpr auto DASH_INTERVAL = 60;              // ダッシュモーション後のインターバル時間
  constexpr auto DASH_TIME = 50 / 4;              // ダッシュアクションが完了するまでの時間
  constexpr auto DASH_MAX = 280;                  // ダッシュモーションの最大移動距離
  constexpr auto DASH_VEC = DASH_MAX / DASH_TIME; // 移動ベクトル

  constexpr auto DASH_WIDTH1 = 40;
  constexpr auto DASH_WIDTH2 = 30;
  constexpr auto DASH_HEIGHT1 = 10;
  constexpr auto DASH_HEIGHT2 = 70;

  constexpr auto DASH_EFFECT = 150;
  // ノックバック関連
  constexpr auto HIT_MAX = 300;         // 最大移動量
  constexpr auto HIT_FRAME = 60;        // ノックバック時間
  constexpr auto INVINCIBLE_TIME = 120; // 無敵フレーム
  // 蔦登り
  constexpr auto HAND_WIDTH = 25;
  constexpr auto HAND_HEIGHT = 10;
  // 各種モーションの画像数
  constexpr auto PF_IDOL = 13;
  constexpr auto PF_RUN = 25;
  constexpr auto PF_DASH = 13;
  constexpr auto PF_JUMP = 6;
  constexpr auto PF_FALL = 10;
  constexpr auto PF_ROB = 13;
  constexpr auto PF_GIVE = 16;
  constexpr auto PF_HIT = 7;
  constexpr auto PF_CLIMB = 13;
  constexpr auto PF_DEATH = 40;
  // 描画切り替えまでに必要なフレーム数
  constexpr auto MF_INTERVAL = 4;
  // 再生時間（Player Motion Frame）
  constexpr auto PMF_IDOL = PF_IDOL * MF_INTERVAL;
  constexpr auto PMF_RUN = PF_RUN * 3;
  constexpr auto PMF_DASH = PF_DASH * MF_INTERVAL;
  constexpr auto PMF_JUMP = PF_JUMP * MF_INTERVAL;
  constexpr auto PMF_FALL = PF_FALL * MF_INTERVAL;
  constexpr auto PMF_ROB = PF_ROB * MF_INTERVAL;
  constexpr auto PMF_GIVE = PF_GIVE * MF_INTERVAL;
  constexpr auto PMF_HIT = PF_HIT * 9;
  constexpr auto PMF_DEATH = 40 * 2;
  constexpr auto PMF_CLIMB = PF_CLIMB * MF_INTERVAL;

  constexpr auto MOVE_DEFAULT = 1.0;
  constexpr auto MOVE_DEBUFF = 0.5; 

  constexpr auto DEBUFF_MAX = 60 * 5;
  // 棘ダメージ判定フラグ
  constexpr auto DAMAGE_ON = true;
  constexpr auto DAMAGE_OFF = false;
}

namespace inr {

  Player::Player(Game& game) : ObjectBase::ObjectBase(game) {
    _type = ObjectType::PLAYER;
    _aState = ActionState::IDOL;
    _aCount = 0;
    _aFrame = 0;
    _knockBack = 0;
    _dashInterval = 0;
    _judegFrame = 0;
    _debuffCount = 0;
    _moveType = "";
    _landingType = "";
    _moveD = MOVE_DEFAULT;
    _ivxInterval = DAMAGE_OFF;
    _souls;
    _direction = false;
    _ivx = std::make_pair(0, 0);
    _changeGraph = true;
    _input = false;
    _isJump = false;
    _sChange = true;
    _jumpPower = 0;
    _position = {0, 0};
    _divKey = std::make_pair(PKEY_IDOL, key::SOUND_NUM);
    _moveVector = { 0, 0 };
    _mainCollision = { _position, (PLAYER_WIDTH / 2), (PLAYER_HEIGHT / 2), true };

    Init();
  }

  Player::~Player() {
    _motionKey.clear();
    _collisions.clear();
  }

  void Player::Init() {
    _invincible = 0; // 無敵時間
    // キー名　first:アニメーションの総フレーム数、second:SEの再生フレーム数
    _motionKey = {
          {PKEY_DEATH, {PMF_DEATH, 50}},
          {PKEY_IDOL, {PMF_IDOL, SE_NUM}}, 
          {PKEY_RUN, {PMF_RUN, SE_RUN1}}, 
          {PKEY_DASH, {PMF_DASH, 50}},
          {PKEY_JUMP, {PMF_JUMP, 50}},
          {PKEY_FALL, {PMF_FALL, 50}},
          {PKEY_ROB, {PMF_ROB, 10}}, 
          {PKEY_GIVE, {PMF_GIVE, 10}},
          {PKEY_HIT, {PMF_HIT, 50}},
          {PKEY_CLIMB, {PMF_CLIMB, 0}},
    };

    auto x = _position.GetX();
    auto y = _position.GetY();

    AABB dashBox = { _position, 25, 55, true };
    AABB robBox = { _position, ROB_WIDTH1, ROB_WIDTH2, ROB_HEIGHT1, ROB_HEIGHT2 };
    AABB giveBox = { _position, GIVE_WIDTH1, GIVE_WIDTH2, GIVE_HEIGHT1, GIVE_HEIGHT2 };
    AABB handBox = { _position, HAND_WIDTH, HAND_HEIGHT, true };
    
    _collisions = { {PKEY_ROB, {robBox}},
            {PKEY_GIVE, {giveBox}},
            {PKEY_DASH, {dashBox}},
            {PKEY_CLIMB, {handBox}},
    };
}

  void Player::Process() {
    if(_aState != ActionState::DEATH) ObjectBase::Process();
    if (_sChange == true) InputOn();
    // 座標戻し後、入力を受け付けるかどうかの判定
    if (_ivxInterval == DAMAGE_ON && _invincible == 60) {
      // 入力受付の再開
      _input = true;
      _ivxInterval = DAMAGE_OFF;
    }

    if (Dead() == true) return;
    // 入力情報を取得
    auto leverLR = _game.GetLeverLR();
    auto leverUD = _game.GetLeverUD();
    auto key = _game.GetTrgKey();
    // デバフ処理がある場合
    if (_moveD == MOVE_DEBUFF) {
      if (_debuffCount == 0) {
        _moveD = MOVE_DEFAULT;
      } else --_debuffCount;
    }

    Move(leverLR);    // 移動処理（レバー入力受付のため停止中）
    Climb(leverUD);   // 上下移動（レバー入力受付のため停止中）
    Action(key);      // アクション
    Grab();           // 掴み
    Dash();           // ダッシュ
    Jump();           // ジャンプ処理
    PositionUpdate(); // 位置座標の更新
    // 各種衝突処理
    auto&& objs = _game.GetObjectServer()->GetEnemys();

    auto cBox = _collisions.find(_divKey.first);
    if (cBox != _collisions.end()) {
      for (auto&& obj : objs) {
        obj->CollisionHit(_divKey.first, cBox->second, _direction);
      }
    }
    auto&& crystal = _game.GetGimmickServer()->GetGimmicks();
    if (cBox != _collisions.end()) {
      for (auto obj : crystal) {
        if (obj->GimmickType() != gimmick::CRYSTAL) continue;
        std::dynamic_pointer_cast<Crystal>(obj)->CollisionHit(_divKey.first, cBox->second, _direction);
      }
    }
  }

  void Player::Draw() {
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();

    int graph;             // グラフィックハンドル格納用
    GraphResearch(&graph); // ハンドル取得
    DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
    DrawDebugBox(_mainCollision);
    std::string& key = _divKey.first;
    auto box = _collisions.find(key);
    if (box != _collisions.end()) {
      if (box->second.GetDrawFlag() == true) {
        DrawDebugBox(box->second, GetColor(255, 0, 0));
      }
    }
#endif
  }

  void Player::SetParameter(ObjectValue objValue) {
    _oValue = objValue;
    _position = _oValue.Positions()[0];
    _direction = _oValue.Direction();
    _sChange = true;

    auto it = _collisions.find(_divKey.first);
    if (it != _collisions.end()) it->second.Update(_position, _direction);
    if (_souls.empty() == true) {
      auto addSoul = _souls;
      for (auto i = 0; i < _souls.size(); ++i) {
        _game.GetObjectServer()->Add(addSoul.front());
        addSoul.pop();
      }
    }

    _mainCollision.Update(_position, _direction);
    _moveType = key::SOUND_PLAYER_RUN1;
    _landingType = key::SOUND_PLAYER_FALL1;
  }

  void Player::SetParameter(std::pair<Vector2, bool> newData, std::string sKey) {
    _oValue.PositionsUpdate(newData.first);
    _oValue.DirectionUpdate(newData.second);
    _position = newData.first;
    _ivx = std::make_pair(0, 0);
    _direction = newData.second;
    _sChange = true;
    _input = false;
    ChangeState(ActionState::IDOL, PKEY_IDOL);
    // 魂が空ではない場合、対応する魂をオブジェクトサーバーに再登録する
    if (_souls.empty() != true) {
      auto addSoul = _souls;
      for (auto i = 0; i < _souls.size(); ++i) {
        addSoul.front()->SetSpwan(_position);
        _game.GetObjectServer()->Add(std::move(addSoul.front()));
        addSoul.pop();
      }
    }

    auto it = _collisions.find(_divKey.first);
    if (it != _collisions.end()) it->second.Update(_position, _direction);

    _mainCollision.Update(_position, _direction);
    _game.GetMapChips()->WorldUpdate(_position);

    if (sKey == stage::STAGE_0 || sKey == stage::STAGE_T ||  sKey == stage::STAGE_1) {
      _moveType = key::SOUND_PLAYER_RUN1;
      _landingType = key::SOUND_PLAYER_FALL1;
    }
    else if (_moveType != key::SOUND_PLAYER_RUN2) {
      _moveType = key::SOUND_PLAYER_RUN2;
      _landingType = key::SOUND_PLAYER_FALL2;
    }
  }

  void Player::StateUpdate() {
    if (0 < _invincible) --_invincible; // 無敵時間がある場合は減少させる
    // 各種モーションに合わせた修正
    switch (_aState) {
    // 移動時
    case ActionState::MOVE:
      if (!_stand) {
        ChangeState(ActionState::FALL, PKEY_FALL);
      }
      return;
    // ジャンプ時
    case ActionState::JUMP:
      // 加速値がプラスの場合
      if (0 < _gravity) {
        _changeGraph = true;
        _aState = ActionState::FALL;
        _divKey.first = PKEY_FALL;
      }
      else if (_stand) { // 重力加速がない場合はアイドル状態に遷移
        ChangeState(ActionState::IDOL, PKEY_IDOL);
      }
      return;
    // 落下時
    case ActionState::FALL:
      // 立っていてる場合
      if (_stand) {
        auto sound1 = SoundResearch(_landingType);
        if (CheckSoundMem(sound1) == 0) {
          auto soundType = se::SoundServer::GetPlayType(_divKey.second);
          PlaySoundMem(sound1, soundType);
        }
        ChangeState(ActionState::IDOL, PKEY_IDOL);
      }
      return;
    // 奪うアクション時
    case ActionState::ROB:
      // 描画更新がある場合は処理から抜ける
      if (_changeGraph == true) break;
      if(0 <= _judegFrame) --_judegFrame; // カウンタ減少
      // 判定カウンタはゼロになったか？
      if (_judegFrame == 0) {
        auto it = _collisions.find(_divKey.first);
        // 当たり判定を止める
        it->second.SetCollisionFlag() = false;
#ifdef _DEBUG
        it->second.SetDrawFlag() = false;
#endif
      } 
      // アニメーションカウンタはマックスになったか？
      if (AnimationCountMax()) {
        ChangeState(ActionState::IDOL, PKEY_IDOL);
        _input = true;  // 入力を受け付ける
      }
      break;
    case ActionState::GIVE:
      if (_changeGraph == true) break;
      if (0 <= _judegFrame) --_judegFrame; // カウンタ減少
      // 判定カウンタはゼロになったか？
      if (_judegFrame == 0) {
        auto it = _collisions.find(_divKey.first);
        // 当たり判定を止める
        it->second.SetCollisionFlag() = false;
        auto sound = se::SoundServer::GetSound(key::SOUND_PLAYER_GIVE_FALSE);
        PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
#ifdef _DEBUG
        it->second.SetDrawFlag() = false;
#endif
      }
      // アニメーションカウンタはマックスになったか？
      if (AnimationCountMax()) {
        ChangeState(ActionState::IDOL, PKEY_IDOL);
        _input = true; // 入力を受け付ける
      }
      break;
    // ノックバック中
    case ActionState::HIT:
      if (_invincible == HIT_FRAME) {
        ChangeState(ActionState::IDOL, PKEY_IDOL);
        _input = true;
        break;
      }
      _moveVector.GetPX() = _knockBack / HIT_FRAME; // 移動量
      break;
    case ActionState::GRAB:
      if (_grab == false || _input == false) {
        _grab = false;
        if (0 <= _gravity) ChangeState(ActionState::FALL, PKEY_FALL);
        else if(_stand == true) ChangeState(ActionState::IDOL, PKEY_IDOL);
      }
      break;
    default:
      return;
    }
  }

  bool Player::Action(int key) {
    // 前フレームの情報
    auto beforeState = _aState;
    // keyに入力情報がある場合
    if (key) {
      // 入力情報に応じた処理を実行
      switch (key) {
      case PAD_INPUT_1: // Xボタンが押された場合、「魂を奪う」
        Rob();          //　奪うアクション実行
        break;
      case PAD_INPUT_2: // Yボタンが押された場合、「魂を与える」
        if (_aState == ActionState::GIVE || _aState == ActionState::ROB) break;
        Give();         // 与えるアクション実行
        break;
      case PAD_INPUT_3: // Aボタンが押された場合、「ジャンプ」
        InputJump();
        break;
      case PAD_INPUT_5: // L1が押された場合、「魂を切り替える」
        ChangeSoul();
        break;
      case PAD_INPUT_6: // R1が押された場合、「ダッシュ」
        if (_aState == ActionState::HIT) break;
        InputDash();
        break;
      }
    }
    // アイドル状態以外で、アニメーションが終わってない場合
    StateUpdate();
    return false;
  }

  void Player::IsGrab() {
    // 蔦と接触しているかどうか
    if (_input != true) return; //　入力を受け付けていない場合は、掴めない
    if (_grab == true) return;
    // Bボタン入力があった場合、蔦登り状態に遷移する
    if (_game.GetTrgKey() == PAD_INPUT_4) {
      ChangeState(ActionState::GRAB, PKEY_CLIMB);
      _grab = true;
    }
  }

  void Player::Grab() {
    auto hand = _collisions.find(PKEY_CLIMB);
    if (_game.GetMapChips()->HitIvy(hand->second, _position, _moveVector, &_ivx,_direction)) IsGrab();
    else _grab = false;
  }

  void Player::Move(int lever) {
    // 入力可能か？
    if (_input == true) {
      // 状態がアイドル、またはモーブの時だけ移動処理を行う。
      auto direction = _direction;
      if (lever < -20) _direction = PL_LEFT;
      else if (20 < lever) _direction = PL_RIGHT;

      if (_aState != ActionState::GRAB) {
        if (_aState != ActionState::FALL && _aState == ActionState::IDOL || _aState == ActionState::MOVE) {
          // 入力情報がある場合
          if (lever < -100 || 100 < lever) {
            // moveではない時、キーと状態を更新
            if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
              ChangeState(ActionState::MOVE, PKEY_RUN);
              _aCount = 0;
            }
            // SEを鳴らすかどうか？
            if (_aCount % GetSoundFrame(_divKey.first) == 0 && IsAnimationMax() != true) {
              auto sound1 = SoundResearch(_moveType);
              if (CheckSoundMem(sound1) == 0 && CheckSoundMem(se::SoundServer::GetSound(key::SOUND_PLAYER_FALL1)) == 0) {
                auto soundType = se::SoundServer::GetPlayType(_divKey.second);
                PlaySoundMem(sound1, soundType);
              }
            }
          }
          else if (_aState == ActionState::MOVE) {
            switch (_stand) {
            case true: // 立っている場合
              ChangeState(ActionState::IDOL, PKEY_IDOL);
              _speed = 0;
              break;
            }
            return;
          }
        }
        // 座標変更
        _speed = (lever * MAX_SPPED) / 1000;
        // 移動ベクトル代入
        _moveVector.GetPX() = _moveD * _speed;
        _speed = 0;
      } else if (_aState == ActionState::GRAB && _grab == true){
        switch (_direction) {
        case enemy::MOVE_LEFT:
          _position.GetPX() = _ivx.second + (_mainCollision.GetWidthMax() / 2);
          return;
        case enemy::MOVE_RIGHT:
          _position.GetPX() = _ivx.first - (_mainCollision.GetWidthMin() / 2);
        }
      }
    }
  }

  void Player::InputDash() {
    // 入力可能状態かつ、インターバル中ではないか？
    if (_input == true && !_dashInterval) {
      // ダッシュ状態ではない場合、各種初期化処理を実行
      if (_aState != ActionState::DASH) {
        ChangeState(ActionState::DASH, PKEY_DASH);
        auto soundKey = SoundResearch(key::SOUND_PLAYER_ROB);
        PlaySoundMem(soundKey, DX_PLAYTYPE_BACK);
        _dashX = DASH_MAX;
        _input = false; // 他アクションの入力を停止する
        _grab = false;

        double fix = 0;
        (_direction == enemy::MOVE_LEFT) ? fix = DASH_EFFECT : fix = -DASH_EFFECT;
        Vector2 effPos = { _position.GetX() + fix, _position.GetY() };

        auto eff = std::make_unique<EffectBase>(_game.GetGame(), effect::DASH, effPos, 30, _direction);
        _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eff), effect::type::BACK);
      }
    }
  }

  void Player::InputJump() {
    // 入力可能状態かつ、立っている場合のみ実行可能
    if (_input == true && (_stand == true || _grab == true)) {
      _jumpPower += 1; // 溜めカウンタを増やす
      _gravity = -_jumpPower;
      // Aキーの入力がない場合、ジャンプを実行
      ChangeState(ActionState::JUMP, PKEY_JUMP);
      auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
      PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

      Vector2 posj = { _position.GetX(), _position.GetY() + JUMP_EFFECT_Y };

      auto eff = std::make_unique<EffectBase>(_game.GetGame(), effect::JUMP, posj, 30);
      _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eff), effect::type::BACK);

      _grab = false;
      _isJump = true;
      _stand = false;
    }
  }

  void Player::Dash() {
    // インターバルがある場合は減らす
    if (0 < _dashInterval) --_dashInterval;
    // ダッシュ状態ではない場合、処理を中断
    if (_input == true) return;

    if (_aState == ActionState::DASH) {
      double dashVec; // 移動ベクトル
    // 向いている向きに応じて代入するベクトルを切り替え
      (_direction == PL_LEFT) ? dashVec = -DASH_VEC : dashVec = DASH_VEC;
      _moveVector.GetPX() = dashVec;
      _dashX -= DASH_VEC;

      auto nextPos = _position.GetX() + dashVec; // 移動後の座標を取得
      _gravity = 0; // ダッシュ中は重力処理無効
      bool moved = _dashX <= 0;  
      // ダッシュ処理は完了したかどうか？
      if (moved) {
        if (_sChange != true) {
          _input = true;
          _dashInterval = DASH_INTERVAL;
        }
        // 立っているかどうかで次のモーションを判定
        if (_stand) {
          ChangeState(ActionState::IDOL, PKEY_IDOL);
          return;
        }
        else {
          ChangeState(ActionState::FALL, PKEY_FALL);
        }
      }
    }
  }

  void Player::Climb(int leverUD) {
    if (_aState != ActionState::GRAB) return; // 掴みフラグがオンではない場合は処理を行わない
    if (-50 < leverUD && leverUD < 50) return;    
    // 座標変更
    double spd = (leverUD * MAX_SPPED) / 1000.0;
    // 移動ベクトル代入
    _moveVector.GetPY() = 1.0 * spd;
  }

  void Player::Jump() {
    if (_jumpPower && _isJump == true) {
        // 溜めはあるか？
          auto pressKey = _game.GetKey();
          if (pressKey & PAD_INPUT_3) {
            // 溜めカウンタを増やす
            _jumpPower += 1;
            auto jumpPower = JUMP_VECTOR * (1.0 + JUMP_MAX);
            _gravity = -jumpPower;
            // 溜めカウンタがマックスではない場合、処理から抜ける
            if (JUMP_MAX <= _jumpPower) {
              _jumpPower = 0;
              _isJump = false;
            }
            return;
          }  
          if (_jumpPower < JUMP_MIN) { 
            _jumpPower = JUMP_MIN; 
          } else {
            auto jumpPower = JUMP_VECTOR * (1.0 + _jumpPower);
            _gravity = -jumpPower;
            _jumpPower = 0;
            _isJump = false;
            return;
          }
            auto jumpPower = JUMP_VECTOR * (1.0 + _jumpPower);
            _gravity = -jumpPower;          
      }
  }

  void Player::Rob() {
    if (_input == true) {
      ChangeState(ActionState::ROB, PKEY_ROB);
      // SE読み込み
      auto sound1 = SoundResearch(key::SOUND_PLAYER_ROB);
      PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

      auto it = _collisions.find(PKEY_ROB);
      it->second.SetCollisionFlag() = true; // 判定オン
#ifdef _DEBUG
      it->second.SetDrawFlag() = true;
#endif
      // エフェクトを作成
      auto robEff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::ROB, _position, 27, _direction);
      robEff->Set(this, _mainCollision.GetWidthMax() * 2);
      _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(robEff), effect::type::FORMER);
      }
      _input = false;              // 入力を受け付けなくする
      _judegFrame = ROB_JUDGEMENT; // 判定カウンタ
  }

  void Player::Give() {
    if (_input == true) {
      ChangeState(ActionState::GIVE, PKEY_GIVE);
      auto giveEff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::GIVE, _position, 60, _direction);
      giveEff->Set(this, 60);
      _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(giveEff), effect::type::FORMER);

      auto it = _collisions.find(PKEY_GIVE);
      it->second.SetCollisionFlag() = true; // 判定オン
#ifdef _DEBUG
      it->second.SetDrawFlag() = true;
#endif
      _input = false;                       // 入力を受け付けなくする
      _judegFrame = ROB_JUDGEMENT;          // 判定カウンタ
    }
  }

  void Player::ChangeSoul() {
    if (_input == false) return;
    if (2 <= _souls.size()) {
      _souls.push(_souls.front());
      _souls.pop();
      auto sound = se::SoundServer::GetSound(key::SOUND_PLAYER_SWITCH);
      PlaySoundMem(sound, se::SoundServer::GetPlayType(key::SOUND_PLAYER_SWITCH));
    }
  }

  bool Player::Damage(bool mv) {
    // 無敵時間中ではない場合のみ、
    if (_invincible == 0) {
      // ダメージ処理
      _input = false; // 入力処理を弾く

      if (_souls.empty() == false) {
        _souls.front()->Del();
        _souls.pop();
        KnockBack(mv);
      }
      else {
        // 空の場合は死亡処理を行う
        Death();
        return true;
      }
    }
    return false;
  }
  // ノックバック
  bool Player::KnockBack(bool mv) {
    if (_invincible != 0) return false;
    _input = false;
    ChangeState(ActionState::HIT, PKEY_HIT);
    auto soundKey = SoundResearch(key::SOUND_PLAYER_HIT);
    auto soundType = se::SoundServer::GetPlayType(_divKey.second);
    PlaySoundMem(soundKey, se::SoundServer::GetPlayType(_divKey.second));

    auto hitEff = std::make_unique<EffectBase>(_game.GetGame(), effect::HIT, _position, 45);
    _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hitEff), effect::type::BACK);
    // ノックバック量（方向の設定）
    switch (mv) {
      // 左に居る場合
    case false:
      _knockBack = HIT_MAX;
      break;
      // 右に居る場合
    case true:
      _knockBack = -HIT_MAX;
      break;
    }
    _invincible = INVINCIBLE_TIME; // 無敵時間を設定
    return true;
  }
  // デバフ
  bool Player::Debuff() {
    // 一定時間の間、移動量(ジャンプ・ダッシュは除く)を減少させる
    _debuffCount = DEBUFF_MAX;
    _moveD = MOVE_DEBUFF;
    // デバフエフェクトの生成
    auto debuffEff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::DEBUFF, _position, effect::DEBUF_MAX * 2);
    debuffEff->Set(this);
    debuffEff->SetLoop(9);
    _game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(debuffEff), effect::type::FORMER);
    return true;
  }

  bool Player::Dead() {
    if (_aState == ActionState::DEATH) {
      if (IsAnimationMax() == true) {
        _game.GetModeServer()->GetModeMain()->GameOver(); // 現在のステージの初期化を行う
      } else AnimationCount();
      return true;
    }
    else AnimationCount();
    return false;
  }

  void Player::Death() {
    auto sound = se::SoundServer::GetSound(key::SOUND_PLAYER_DEAD);
    PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
    // 死亡処理（アニメーションを挟む）
    ChangeState(ActionState::DEATH, PKEY_DEATH); // 死亡状態に切り替える
    _input = false;                              // 入力を無効化する
    _mainCollision.SetCollisionFlag() = false;   // 当たり判定が機能しないようにオフにする
    auto box = _collisions.find(PKEY_DASH);
    box->second.SetCollisionFlag() = false;
#ifdef _DEBUG
    _mainCollision.SetDrawFlag() = false;
    box->second.SetDrawFlag() = false;
#endif
  }

  void Player::PositionUpdate() {
    // 移動ベクトルYに加速度を代入
    if (_aState != ActionState::GRAB) _moveVector.GetPY() = _gravity;
    auto isGran = false;
    // マップチップにめり込んでいる場合は座標を修正
    auto hitChip = _game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction, &isGran);
    if (hitChip == mapchip::THORM) DamageThorm(); // 横から接触した場合もダメージ処理を呼び出す
    // ギミックにめり込んでいるか
    GimmickCheck(_moveVector);

    _position = _position + _moveVector; // 位置座標を更新
    auto mcsw = _game.GetMapChips()->GetMapSizeWidth();

    if (_mainCollision.GetMin().GetX() < 0) {
      _position.GetPX() = _mainCollision.GetWidthMax();
    }
    else if (_game.GetMapChips()->GetMapSizeWidth() < _mainCollision.GetMax().GetX() && 0 < _moveVector.GetX()) {
      _position.GetPX() = mcsw - _mainCollision.GetWidthMax();
    }
    // 各種当たり判定の更新
    auto it = _collisions.find(_divKey.first);
    if (it != _collisions.end()) {
      it->second.Update(_position, _direction);
    }

    if (_aState == ActionState::DASH) {
      _mainCollision.Swap(it->second);
    } else {
      _mainCollision.Update(_position, _direction);
    }
    // 魂の目標距離を変更
    if (!_souls.empty()) {
      auto ss = _souls;
      for (auto i = 1; i <= _souls.size(); ++i) {
        ss.front()->Space(i * 60);
        ss.pop();
      }
    }
    // 生存している間のみ、座標を切り替える
    if (_aState != ActionState::DEATH && _game.GetModeServer()->GetModeMain()->BossFlag() != true) { 
      // エンディング遷移フラグがない場合
      if (_game.GetModeServer()->GetModeMain()->EndingFlag() != true) {
        _game.GetMapChips()->WorldUpdate(_position);
      }
    }
    // 移動ベクトル初期化
    _moveVector = { 0, 0 };
  }

  void Player::ChangeState(Player::ActionState nextState, std::string key) {
    _aState = nextState;
    _divKey.first = key;
    _changeGraph = true;
  }

  void Player::SoulCatch(std::shared_ptr<SoulSkin> soul) {
    _souls.push(std::move(soul));
  }

  std::shared_ptr<SoulSkin> Player::GiveSoul() {
    auto giveSoul = _souls.front();
    _souls.pop();  
    return std::move(giveSoul);
  }

  bool Player::IsStandChip() {
    auto nowCol = NowCollision(_divKey.first);
    auto chipType = _game.GetMapChips()->IsStand(nowCol, _position, _gravity, &_lastChip, ThisPlayer());
    switch (chipType) {
    case mapchip::IVY:
    case mapchip::NONE:
      return false;
    case mapchip::THORM:
      DamageThorm(); // 蔦と接触している場合はダメージ処理呼び出し
      return true;
    default:
      return true;
    }
  }

  void Player::DamageThorm() {
    // 魂は空かどうか？
    if (_souls.empty() == true) {
      Death(); // 空の場合は死亡処理呼び出し
      return;
    }
    else {
      _souls.front()->Del();
      _souls.pop();

      auto soundKey = SoundResearch(key::SOUND_PLAYER_HIT);
      auto soundType = se::SoundServer::GetPlayType(_divKey.second);
      PlaySoundMem(soundKey, se::SoundServer::GetPlayType(_divKey.second));
    }
    _invincible = INVINCIBLE_TIME; // 無敵時間を設定
    _position = _lastChip;         // 座標切り替え
    // 移動後にバグが発生しないように調整
    _knockBack = 0;
    _dashX = 0;
    _gravity = 0;

    _ivxInterval = DAMAGE_ON;
    _invincible = 120;
    _input = false;
  }

  bool Player::Reset() {
    // 各種初期化処理実行
    _input = false;
    _sChange = true;
    _debuffCount = 0;
    _moveD = MOVE_DEFAULT;
    _ivx = std::make_pair(0, 0);
    _position = _oValue.Positions().at(0);
    _direction = _oValue.Direction();
    _mainCollision.Update(_position, _direction);
    _mainCollision.SetCollisionFlag() = true;

    std::queue<std::shared_ptr<SoulSkin>> sss;
    _souls = sss;

    auto dashCol = _collisions.find(PKEY_DASH);
    dashCol->second.Update(_position, _direction);
    dashCol->second.SetCollisionFlag() = true;
#ifdef _DEBUG
    _mainCollision.SetDrawFlag() = true;
    dashCol->second.SetDrawFlag() = true;
#endif
    ChangeState(ActionState::IDOL, PKEY_IDOL);
    _grab = false;
    _isJump = false;
    return true;
  }

  AABB Player::NowCollision(std::string key) {
    // 現在のアクション状態はボックスを修正する必要があるか？
    if (_aState == ActionState::DASH) {
      auto it = _collisions.find(key);
      auto box = it->second;
      box.Update(_position, _direction);
      return box;
    }
    return _mainCollision;
  }

  void Player::InputOn() {
    if (_game.GetModeServer()->IsFadeEnd() != true) return;
    // 入力を再開する
    _input = true;
    _sChange = false;
  }

#ifdef _DEBUG
  void Player::DebugInfo() {
    DrawFormatString(0, 0, GetColor(255, 0, 255), "ActionStatet : %d\n", _aState);
    DrawFormatString(0, 25, GetColor(255, 0, 255), "Animation : %d\n", _aCount);
    ObjectBase::DrawDebugBox(_mainCollision);
    DrawFormatString(0, 100, GetColor(255, 0, 0), "maincollision（minX:%d, minY:%d）\n", _mainCollision.GetMin().IntX(), _mainCollision.GetMin().IntY());
    DrawFormatString(0, 125, GetColor(255, 0, 0), "maincollision（maxX:%d, maxY:%d）\n", _mainCollision.GetMax().IntX(), _mainCollision.GetMin().IntY());
    DrawFormatString(0, 150, GetColor(255, 0, 0), "pos.x = %d\n", _position.IntX());
    DrawFormatString(0, 175, GetColor(255, 0, 0), "pos.y = %d\n", _position.IntY());
    DrawFormatString(0, 200, GetColor(255, 0, 0), "stand = %d\n", _stand);
    DrawFormatString(0, 225, GetColor(255, 0, 0), "_gravity = %d\n", _gravity);
  }
#endif
}