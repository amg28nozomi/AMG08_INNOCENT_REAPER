/*****************************************************************//**
 * @file   Collision.cpp
 * @brief  軸平行境界ボックスのスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flag) : _minV(min), _maxV(max),_collisionFalg(flag){
#ifdef _DEBUG
  _drawFlag = true; // 描画フラグをオン
#endif
  // ボックスの横幅と立幅を算出
  int width = static_cast<int>(_maxV.GetX() - _minV.GetX());
  int height = static_cast<int>(_maxV.GetX() - _minV.GetX());
  // 各種修正値代入
  _widthMin = width;
  _widthMax = width;
  _heightMin = height;
  _heightMax = height;
  // 中心座標に登録
  auto posX = max.GetX() - min.GetX() / 2;
  auto posY = max.GetY() - min.GetY() / 2;
  _center = { posX, posY };
}

Collision::Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag) {
  // 各種初期化
  _widthMin = width1;
  _widthMax = width2;
  _heightMin = height1;
  _heightMax = height2;
  _collisionFalg = flag;
#ifdef _DEBUG
  _drawFlag = true;
#endif
  _minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
  _maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision(Vector2& pos, int width, int height, bool flag) {
  // 各種初期化
  _widthMin = width;
  _widthMax = width;
  _heightMin = height;
  _heightMax = height;
  _collisionFalg = flag;
#ifdef _DEBUG
  _drawFlag = true;
#endif
  _minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
  _maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision() {
  // 各種初期化
  _widthMin = 0;
  _widthMax = 0;
  _heightMin = 0;
  _heightMax = 0;
  _collisionFalg = false;
#ifdef _DEBUG
  _drawFlag = true;
#endif
  _minV = {};
  _maxV = {};
}

void Collision::Update(Vector2& pos, bool inv) {
  // 向きに応じて当たり判定のx座標を変更する
  _minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
  _maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
  _center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::Swap(Collision col){
  // 当たり判定の交換および修正
  _minV = col.GetMin();
  _maxV = col.GetMax();
  _center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::DrawColorBox(int color) {
  // 描画座標の算出
  auto minX = _minV.IntX();
  auto minY = _minV.IntY();
  auto maxX = _maxV.IntX();
  auto maxY = _maxV.IntY();
  // 描画処理呼び出し
  DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
  // 判定フラグはオンになっているか？
  if (_collisionFalg == true && collision._collisionFalg == true) {
    // 接触しているか？
    bool flag = _maxV.GetX() < collision._minV.GetX() ||
      collision._maxV.GetX() < _minV.GetX() ||
      _maxV.GetY() < collision._minV.GetY() ||
      collision._maxV.GetY() < _minV.GetY();
    return !flag;
  }
  return false;    // 判定フラグがどちらか片方でもオフなら当たらない
}