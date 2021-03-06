/*****************************************************************//**
 * @file   GimmickBase.cpp
 * @brief  ギミックのスーパークラス（オブジェクトベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "GimmickBase.h"
#include "Game.h"

namespace inr {

  GimmickBase::GimmickBase(Game& game) : ObjectBase(game), _gType(GimmickType::LEVER) {
    // 初期化
    _type = ObjectType::GIMMICK;
    _gType = GimmickType::LEVER;
  }

  GimmickBase::~GimmickBase() {
  }

  void GimmickBase::Init() {
  }

  void GimmickBase::Process() {
  }

  void GimmickBase::Draw() {
    // 描画座標の算出
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // グラフィックハンドルの取得
    int graph;  
    GraphResearch(&graph);
    // 描画
    DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
  }

  int GimmickBase::GimmickType() {
    // ギミックの種類に応じた値を返す
    switch (_gType) {
    case GimmickType::LEVER:
      return gimmick::LEVER;   // レバー
    case GimmickType::BLOCK:
      return gimmick::BLOCK;   // 壊れる岩
    case GimmickType::CRYSTAL:
      return gimmick::CRYSTAL; // 水晶
    case GimmickType::DOOR:
      return gimmick::DOOR;    // ドア
    }
  }

  void GimmickBase::ObjValueUpdate() {
  }
}