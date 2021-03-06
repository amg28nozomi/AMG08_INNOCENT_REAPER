/*****************************************************************//**
 * @file   TrackingEffect.cpp
 * @brief  追跡エフェクト（エフェクトベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "TrackingEffect.h"
#include "ObjectBase.h"

namespace inr {

  TrackingEffect::TrackingEffect(Game& game, const std::string gh, const Vector2 spawnPos, const int maxFrame, const bool direction) : EffectBase(game, gh, spawnPos, maxFrame, direction) {
    _target = nullptr;
    _fix = { 0, 0 };
  }

  void TrackingEffect::Set(ObjectBase* target, double fixx, double fixy) {
    _target = target;
    _fix = { fixx, fixy };
  }

  void TrackingEffect::Process() {
    auto pos = _target->GetPosition();
    switch (_direction) {
    case false:
      pos.GetPX() += _fix.GetX();
      pos.GetPY() = pos.GetY() + _fix.GetY();
      break;
    case true:
      pos.GetPX() -= _fix.GetX();
      pos.GetPY() = pos.GetY() + _fix.GetY();
      break;
    }
    _position = pos;
    EffectBase::Process();
  }
}