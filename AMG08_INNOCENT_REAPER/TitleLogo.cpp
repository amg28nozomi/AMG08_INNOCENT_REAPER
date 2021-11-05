/*****************************************************************//**
 * @file   TitleLogo.cpp
 * @brief  タイトルロゴ（画像クラスのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "TitleLogo.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "ModeServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
  constexpr auto PAL_MIN = 0;
  constexpr auto PAL_MAX = 255;

  constexpr auto MAX_RATE = 1.2;
  constexpr auto MIN_RATE = 1.0;
  constexpr auto RATE_VALUE = 0.005;

  constexpr auto RATE_NULL = -1;  // なし
  constexpr auto RATE_SUB = 0;
  constexpr auto RATE_ADD = 1;
}

namespace inr {

  TitleLogo::TitleLogo(Game& game) : Image(game), _hitCol1(), _hitCol2() {
    Init();
  }

  void TitleLogo::Init() {
    _pos = { 960, 800 };
    _position2 = { 960, 675 };
    _rate = { MIN_RATE, MIN_RATE };
    _rateFlag = { RATE_NULL, RATE_NULL };
    _pal = PAL_MIN;
    _graphKey = TITLE_EXIT1;
    _graphKey2 = TITLE_START1;

    _hitCol2 = { _pos, TITLE_START_WIDTH / 4, TITLE_UI_HEIGHT / 4, true };
    _hitCol1 = { _position2, TITLE_EXIT_WIDTH / 2, TITLE_UI_HEIGHT / 4, true };
  }

  void TitleLogo::Process() {
    RateUpdate();
    // 透明度の変更
    if (_pal != PAL_MAX) {
      _pal += 4;
      if (PAL_MAX < _pal) _pal = PAL_MAX;
    }

    auto&& objs = _game.GetObjectServer()->GetObjects();
    for (auto& obj : objs) {
      const auto& collision = obj->GetMainCollision();
      if (_hitCol1.HitCheck(collision)) {
        _rateFlag[0] = RATE_ADD;
        if (_game.GetTrgKey() == PAD_INPUT_3) {
          // ゲーム本編に遷移する
          auto sound = se::SoundServer::GetSound(system::SOUND_SELECT);
          PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUND_SELECT));
          _game.GetModeServer()->ModeChange(mode::MAIN);
          CollisionOut();
        }
      } else _rateFlag[0] = RATE_NULL;
      if (_hitCol2.HitCheck(collision)) {
        _rateFlag[1] = RATE_ADD;
        if (_game.GetTrgKey() == PAD_INPUT_3) {
          auto sound = se::SoundServer::GetSound(system::SOUND_GAME_END);
          PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUND_GAME_END));
          // プログラムを終了する
          _game.GetModeServer()->GameEnd();
          CollisionOut();
        }
      } else _rateFlag[1] = RATE_NULL;
    }
  }

  void TitleLogo::Draw() {
    auto x1 = _pos.IntX();
    auto y1 = _pos.IntY();
    auto x2 = _position2.IntX();
    auto y2 = _position2.IntY();
    auto gh1 = graph::ResourceServer::GetHandles(_graphKey, 0); // 描画するグラフィックハンドルの取得
    auto gh2 = graph::ResourceServer::GetHandles(_graphKey2, 0);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
    DrawRotaGraph(x1, y1, _rate[1], 0, gh1, true, false);
    DrawRotaGraph(x2, y2, _rate[0], 0, gh2, true, false);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
    if (_game.IsDebugMode() == true) {
      auto c = DxLib::GetColor(255, 255, 0);
      DebugBox(_hitCol1);
      DebugBox(_hitCol2);
    }
#endif
  }
#ifdef _DEBUG
  void TitleLogo::DebugBox(Collision col) {
    auto drawFlg = col.SetDrawFlag();
    // 描画判定はオンになっているかどうか？
    if (drawFlg) {
      auto min = col.GetMin();
      auto max = col.GetMax();

      auto minx = min.IntX();
      auto maxx = max.IntX();
      auto miny = min.IntY();
      auto maxy = max.IntY();
      DxLib::DrawBox(minx, miny, maxx, maxy, GetColor(255, 255, 0), FALSE);
    }
  }
#endif
  void TitleLogo::CollisionOut() {
    _hitCol1.SetCollisionFlag() = false;
    _hitCol2.SetCollisionFlag() = false;
  }

  bool TitleLogo::RateUpdate() {
    for (auto i = 0; i < static_cast<int>(_rateFlag.size()); ++i) {
      switch(_rateFlag[i]){
      // 触れていない場合
      case RATE_NULL:
        if (_rate[i] == MIN_RATE) continue;
        _rate[i] -= RATE_VALUE;
        if (_rate[i] < MIN_RATE) _rate[i] = MIN_RATE;
        continue; 
      case RATE_ADD:
        if (_rate[i] == MAX_RATE) {
          _rateFlag[i] = RATE_SUB;
          continue;
        }
        _rate[i] += RATE_VALUE;
        if (MAX_RATE < _rate[i]) _rate[i] = MIN_RATE;
        continue;
      case RATE_SUB:
        if (_rate[i] == RATE_SUB) {
          _rateFlag[i] = RATE_ADD;
          continue;
        }
        _rate[i] -= RATE_VALUE;
        if (MIN_RATE < _rate[i]) _rate[i] = MIN_RATE;
        continue;
      }
    }
    return true;
  }
}