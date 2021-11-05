/*****************************************************************//**
 * @file   ObjectBase.cpp
 * @brief  オブジェクトのスーパークラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ObjectBase.h"
#include "Collision.h"
#include "Vector2.h"
#include "Game.h"
#include "MapChips.h"
#include "EnemyBase.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "ChipNumber.h"
#include "GimmickBase.h"
#include "Door.h"
#include "Block.h"
#include <DxLib.h>
#include <memory>

namespace {
  constexpr auto SPEED_NULL = 0;
  constexpr auto TEST_VALUE = 15;
}

namespace inr {

  ObjectBase::ObjectBase(Game& game) : _game(game), _mainCollision(Vector2(), Vector2()), _oValue() {
    _delete = false;
    _isJump = false;
    _lastChip = { 0, 0 };
    Init();
  }

  ObjectBase::~ObjectBase() {
  }

  void ObjectBase::Init() {
    _type = ObjectType::OBJECT_BASE;
    _speed = SPEED_NULL;
    _aCount = 0;
    _gravity = 1;
    _direction = false;
    _stand = false;
    _changeGraph = true;

    _position = { 0, 0 };
  }

  void ObjectBase::Process() {
    _gravity += FRAME_G; // 加速度を加算
    if (MAX_G < _gravity) _gravity = MAX_G;

    auto nowCol = NowCollision(_divKey.first);
    // マップチップの上に立っているかどうか
    if (IsStandChip()) {
      // 加速度が0の時だけ立っている
      if (0 < _gravity) {
        _stand = true;
      }
      _gravity = 0;
    } else {
      _stand = false;
    }

    auto&& objs = _game.GetObjectServer()->GetEnemys();
    for (auto&& obj : objs) {
      if (obj->GetType() != ObjectType::ENEMY) continue;
      if (obj->IsEmpty() != true) continue;
      // 抜け殻の当たり判定を取得
      auto emptyBox = obj->EmptyBox();
      if (_mainCollision.HitCheck(emptyBox) == true);
        // x座標は範囲内に収まっているか
        if ((emptyBox.GetMin().GetX() < nowCol.GetMin().GetX() && nowCol.GetMin().GetX() < emptyBox.GetMax().GetX()) ||
          (emptyBox.GetMin().GetX() < nowCol.GetMax().GetX() && nowCol.GetMax().GetX() < emptyBox.GetMax().GetX())) {

          if (nowCol.GetMax().GetY() <= emptyBox.GetMin().GetY() + TEST_VALUE && emptyBox.GetMin().GetY() <= nowCol.GetMax().GetY()) {
            _stand = true;
            _isJump = false;
            _gravity = 0;

            auto h = nowCol.GetHeightMax();
            _position.GetPY() = emptyBox.GetMin().GetY() - h;
          }
        }
    }

    if (nowCol.GetMin().GetY() < 0) {
      _gravity = 0;
      _position.GetPY() = nowCol.GetHeightMin();
    }
  }

  void ObjectBase::Draw() {
  }

  int ObjectBase::GetSize(const std::string& key) {
    // 要素検索
    auto it = _motionKey.find(key);
    if (it == _motionKey.end()) {
      // イテレータが見つからなった場合は-1を返す
#ifdef _DEBUG
      OutputDebugString("キーがヒットしませんでした。キー情報を確認してください。\n");
#endif
      return -1;
    }
    return it->second.first;
  }

  void ObjectBase::AnimationCount() {
    // カウンタは総再生時間よりも小さいか？
    if (_aCount < GetSize(_divKey.first)) { 
      // アニメーションが終わっていない場合はカウントを増やす
      ++_aCount; 
      return;
    } else { 
      // カウンター初期化
      _aCount = 0; 
      return;
    }
  }

  int ObjectBase::AnimationInterval() { 
    return GetSize(_divKey.first) / graph::ResourceServer::GetAllNum(_divKey.first); 
  }

  int ObjectBase::AnimationNumber() {
    // グラフィックが切り替わる猶予フレームを算出
    auto interval = AnimationInterval();
    // 何番目のアニメーションが呼び出されているか
    return _aCount / interval % graph::ResourceServer::GetAllNum(_divKey.first);
  }

  bool ObjectBase::IsAnimationMax() {
    // 現在の描画は最後かどうか？
    bool isMax = AnimationNumber() == graph::ResourceServer::GetAllNum(_divKey.first) - 1;
    return isMax;
  }

  bool ObjectBase::AnimationCountMax() {
    if (_aCount == (GetSize(_divKey.first) - 1)) return true;
    return false;
  }

  bool ObjectBase::GraphResearch(int* gh) {
    // フラグがオンの時、描画するグラフィックを切り替える
    if (_changeGraph) {
      _changeGraph = false;
      _aCount = 0;
      *gh = graph::ResourceServer::GetHandles(_divKey.first, 0);  // 最初の要素を取得
      return true;
    }
    auto interval = AnimationInterval();
    // 何番目のアニメーションが呼び出されているか
    auto no = AnimationNumber();
    // グラフィックハンドルを読み込む
    *gh = graph::ResourceServer::GetHandles(_divKey.first, no);
    return false;
  }

  int ObjectBase::SoundResearch(const std::string& key) {
    _divKey.second = key;
    auto sound = se::SoundServer::GetSound(_divKey.second);
    return sound;
  }

  int ObjectBase::GetSoundFrame(const std::string& key) {
    // キー検索
    auto it = _motionKey.find(key);
    // ヒットしなかった場合は-1を返す。
    if (it == _motionKey.end()) {
      return -1;
    }
    // SEの再生フレーム時間を取り出して返す。
    auto soundFrame = it->second.second;
    return soundFrame;
  }

#ifdef _DEBUG
  void ObjectBase::DrawDebugBox(Collision cb, int color) {
    if (_game.IsDebugMode() != true) return;  // フラグがオンの時のみ描画処理を実行する
    auto drawFlg = cb.SetDrawFlag();
    // 描画判定はオンになっているかどうか？
    if (drawFlg) {
      auto min = cb.GetMin();
      auto max = cb.GetMax();
      _game.GetMapChips()->Clamp(min);
      _game.GetMapChips()->Clamp(max);

      auto minX = min.IntX();
      auto maxX = max.IntX();
      auto minY = min.IntY();
      auto maxY = max.IntY();
      DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
    }
  }
#endif

  void ObjectBase::GimmickCheck(Vector2& move) {
    auto gimmicks = _game.GetGimmickServer()->GetGimmicks();
    for (auto g : gimmicks) {
      // 押し出し処理があるか？
      if (g->GimmickType() == gimmick::DOOR) {
        auto door = std::dynamic_pointer_cast<Door>(g);
        if(door->IsSwitch() == gimmick::OFF) door->Extrude(NowCollision(_divKey.first), _position, move, _direction);
      }
      else if (g->GimmickType() == gimmick::BLOCK) {
        auto block = std::dynamic_pointer_cast<Block>(g);
        if (block->IsBreak() == gimmick::block::BRAKE_OFF) block->Extrude(NowCollision(_divKey.first), _position, move, _direction);
      }
    }
  }

  bool ObjectBase::IsStandChip() {
    auto nowCol = NowCollision(_divKey.first); // 現在の当たり判定
    auto chipType = _game.GetMapChips()->IsStand(nowCol, _position, _gravity, &_lastChip, ThisPlayer());
    switch (chipType) {
    case mapchip::IVY:
    case mapchip::NONE:
      return false;
    default:
      return true;
    }
  }

  bool ObjectBase::ThisPlayer() {
    switch (_type) {
    case ObjectType::PLAYER:
      return true;
    default:
      return false;
    }
  }

  AABB ObjectBase::NowCollision(std::string key) {
    return AABB(Vector2(), Vector2());
  }

  void ObjectBase::Del() {
    _delete = true;
    if (_game.GetObjectServer()->DelFlag() != true) _game.GetObjectServer()->DelOn();
  }

  void ObjectBase::SetParameter(ObjectValue objValue) {
  }

  bool ObjectBase::IsDead() {
    return false;
  }
}