/*****************************************************************//**
 * @file   Lever.cpp
 * @brief  レバークラス（ギミックベースクラスのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Lever.h"
#include "Game.h"
#include "GimmickServer.h"
#include "SoundServer.h"
#include "Door.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "GimmickValue.h"

namespace inr {

  Lever::Lever(Game& game) : GimmickBase(game) {
    // 初期化
    _gType = GimmickType::LEVER;
    _door.reset();
    _divKey = { gimmick::lever::KEY_LEVER, "" }; // キーの設定
    // アニメーションの設定
    _motionKey = { 
      { _divKey.first, {20, 50}},
      { gimmick::lever::KEY_LEVER_BOSS, {30, 50}}
    };
  }

  void Lever::Process() {
    if (IsAnimationMax()) return; // アニメーション番号が最大の場合は処理を終了
    // 扉が開いている場合はアニメーションを行う
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) ++_aCount;
      return;
    }
    else if (_door->IsSwitch()) ++_aCount;
  }

  void Lever::Draw() {
    // 描画座標の算出
    Vector2 xy = _position;
    _game.GetMapChips()->Clamp(xy);
    auto x = xy.IntX();
    auto y = xy.IntY();
    // グラフィックハンドルの取得
    int graph;
    GraphResearch(&graph);
    DrawRotaGraph(x, y, 1.0, 0, graph, true); // 描画
#ifdef _DEBUG
    DrawDebugBox(_mainCollision); // 当たり判定の描画
#endif
  }

  void Lever::OpenDoor() {
    // 既に扉が開いている場合は処理を中断
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) return;
    }
    else if (_door->IsSwitch() == gimmick::ON ) return; // ドアが開いている場合は処理を抜ける
    // 解放SEを取得して再生を行う
    auto sh = SoundResearch(gimmick::lever::KEY_LEVER);
    PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) { 
      // ボス扉の場合は専用フラグを起動する
      // ボスステージへと続く扉を解放する
      _game.GetModeServer()->GetModeMain()->OpenBossStage();
    }
    else _door->SwitchOn(); // スイッチオン
#ifdef _DEBUG
    _mainCollision.SetDrawFlag() = false; // 当たり判定の描画を終了
#endif
  }

  void Lever::SetParameter(ObjectValue objValue) {
    _oValue = objValue; // 登録
    // 各種パラメータの算出
    _position = _oValue.Positions()[0];
    _mainCollision = {  _position, 30, 30, 50, 70, true  };
    // ボス用レバーの場合は専用処理を行う
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
      _divKey.first = gimmick::lever::KEY_LEVER_BOSS; // グラフィック切り替え
      // 既に扉が開いている場合
      if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) {
        _aCount = AnimationCountMax();
#ifdef _DEBUG
        _mainCollision.SetDrawFlag() = false;
#endif
      }
      return;
    }
    // ドアの生成
    auto gDoor = std::make_shared<Door>(_game.GetGame());
    // 画像のキーの取得
    std::string gh;
    switch (_oValue.GimmickType()) {
    case gimmick::door::D_LEVER:
      gh = gimmick::door::KEY_DOOR_LEVER;
      break;
    case gimmick::door::D_RED:
      gh = gimmick::door::D_RED;
      break;
    case gimmick::door::D_BLUE:
      gh = gimmick::door::D_BLUE;
      break;
    default:
      return;
    }
    // 既に扉が開かれている場合はアニメーション番号と当たり判定を修正
    if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
      _aCount = AnimationCountMax();
#ifdef _DEBUG
      _mainCollision.SetDrawFlag() = false;
#endif
    }
    // 扉を登録する
    gDoor->SetParameter(_oValue.Positions()[1], gh, _oValue.GimmickFlag());
    _door = gDoor; // 登録
    _game.GetGimmickServer()->Add(gDoor); // ギミックサーバに登録
  }

  void Lever::ObjValueUpdate() {
    if (_oValue.GimmickType() == gimmick::door::D_BOSS) return; // 処理を行わない
    auto flag = oscenario::gimmick::FLAG_FALSE;
    // フラグ切り替え
    if (_door->IsSwitch() == gimmick::ON) flag = oscenario::gimmick::FLAG_TRUE;
    _oValue.FlagUpdate(flag); // 登録情報の更新
  }
}