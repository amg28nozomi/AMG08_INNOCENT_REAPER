/*****************************************************************//**
 * @file   StageUi.cpp
 * @brief  ステージUI（パーティクル画像のサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "StageUi.h"
#include "ResourceServer.h"
#include "MapChips.h"
#include <DxLib.h>

namespace inr {

  StageUi::StageUi(Game& game) : Particle_Image(game) {
    _graphKey = image::particle::STAGE_UI;
    _gNumber = 0;
    Init();
  }

  void StageUi::Init() {
    _alive = 0;
    _pos = { 0, 100 };
    _pal = 0;
    _fadeDraw = false;
    _end = false;
    _isDraw = false;
    _animation = animation::A_NORMAL;
  }

  void StageUi::Process() {
    if (_isDraw != true) return;
    IntervalCount();
    Animation();
    
  }

  void StageUi::Draw() {
    if (_isDraw != true) return; // フラグがオフの場合は処理を実行しない
    auto x = _pos.IntX();
    auto y = _pos.IntY();
    auto graph = graph::ResourceServer::GetHandles(_graphKey, _gNumber);

    SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, _pal);
    DrawGraph(x, y, graph, TRUE);
    SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
  }

  void StageUi::SetParameter(std::string graph, Vector2 pos, int pal) {
    _graphKey = graph;
    _pos = pos;
    _pal = pal;
  }

  bool StageUi::ChangeNumber(const std::string stageKey) {
    if (stageKey == stage::STAGE_0 || stageKey == stage::STAGE_T) _gNumber = 0;
    else if (stageKey == stage::STAGE_1) _gNumber = 1;
    else _gNumber = 2;
    Init();
    return true;
  }

  bool StageUi::IntervalCount() {
    if (_animation == animation::A_NORMAL && _end == false) {
      if (_alive == 0) {
        _animation = animation::A_SUB;
        return true;
      }
      else if (0 < _alive) --_alive;
      return false;
    }
    else if (_animation == animation::A_SUB && _end == false) _end = true;
    return false;
  }

  bool StageUi::DrawStart() {
    if (Particle_Image::DrawStart() != true) return false;
    _alive = 60;
    _fadeDraw = true;
    return true;
  }
}