/*****************************************************************//**
 * @file   EffectBase.cpp
 * @brief  エフェクトのスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "EffectBase.h"
#include "EffectBase.h"
#include "MapChips.h"
#include "ResourceServer.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include <DxLib.h>

namespace inr {

  EffectBase::EffectBase(Game& game, const std::string gh, const Vector2 spawnPos, const int maxFrame, bool direction) : _game(game), _position(spawnPos),_collision(Vector2(), Vector2()) {
    _count = 0;
    _graphKey = gh;
    _alive = maxFrame;
    _delete = false;
    _direction = direction;  
    _isDamage = false;
    _loop = 0;
    _dMax = 0;
  }

  void EffectBase::Init() {
  }

  void EffectBase::Process() {
    ++_count; // カウンタ加算  
    // ダメージ判定がある場合はダメージ処理を呼び出し
    if(IsDamage() == true) Damage();
    // カウンタが生存時間に到達した場合
    if (_count == (_alive - 1)) {
      // ループ処理がない場合は消去処理を行う
      if (_loop == 0) {
        _delete = true; // 消去フラグをオンにする（消去予約）
        _game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
        return;
      }
      --_loop;    // ループ減算
      _count = 0; // カウンタ初期化
    }
  }

  void EffectBase::Draw() {
    // 描画座標の算出
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // グラフィックハンドルの取得
    int graph;
    GraphResearch(&graph);
    DrawRotaGraph(x, y, 1.0, 0, graph, TRUE, _direction);
#ifdef _DEBUG
    if (_game.IsDebugMode() != true) return;
    if (_isDamage != true) return;
    // デバッグモードかつ、ダメージ判定がある場合のみ当たり判定の描画を行う
    auto db = _collision;
    auto min = db.GetMin();
    auto max = db.GetMax();
    // 描画座標のクランプ
    _game.GetMapChips()->Clamp(min);
    _game.GetMapChips()->Clamp(max);
    DxLib::DrawBox(min.IntX(), min.IntY(), max.IntX(), max.IntY(), GetColor(255, 255, 255), FALSE);
#endif
  }

  int EffectBase::GraphNumber() {
    // 現在の画像の分割数を取得
    auto allNum = graph::ResourceServer::GetAllNum(_graphKey);
    auto interval = _alive / allNum;      // 猶予時間の割り出し
    auto no = _count / interval % allNum; // 描画するアニメーション番号の算出
    return no;
  }

  void EffectBase::GraphResearch(int* gh) {
    *gh = graph::ResourceServer::GetHandles(_graphKey, GraphNumber());
  }

  void EffectBase::SetDamageEffect(int width, int height, int dinter) {
    // ダメージ判定を持たせる
    _isDamage = true; // ダメージオン
    _collision = { _position, width / 2, height / 2, true };
    _dInter = dinter;
  }

  void EffectBase::SetDamageEffect(int width1, int width2, int height1, int height2, int dinter, int max) {
    // 各種ダメージの設定
    _isDamage = true;
    _collision = { _position, width1, width2, height1, height2, true };
    _dInter = dinter;
    // 終了条件が0以下の場合は最後のアニメーション番号を登録
    if (max <= 0) _dMax = graph::ResourceServer::GetAllNum(_graphKey) - 1;
    else _dMax = max; // そうでない場合は直登録
  }

  void EffectBase::SetLoop(int max) {
    if (max <= 0) max = 0; // 引数が0以下の場合は修正
    _loop = max;           // 登録
  }

  void EffectBase::Damage() {
    // 自機
    auto player = _game.GetObjectServer()->GetPlayer();
    if (_collision.HitCheck(player->GetMainCollision()) != true) return;
    player->Damage(IsPlayerPosition()); // 接触判定がある場合はダメージ処理呼び出し
  }

  bool EffectBase::IsPlayerPosition() {
    auto player = _game.GetObjectServer()->GetPlayer();
    // 現在の座標から自機の座標を引いた場合、その値は正負どちらになっているか？
    auto pos = _position.GetX() - player->GetPosition().GetX();
    if (pos < 0) return false; // 左側にいる
    else return true;          // 右側に居る
  }

  bool EffectBase::IsDamage() {
    if (_isDamage != true) return false; // ダメージ判定無し
    // 現在のアニメーション番号はダメージ判定があるか？
    auto no = GraphNumber();
    if (_dInter < no && no <= _dMax) return true; // ダメージ判定有り
    return false; // ダメージ判定無し
  }
}