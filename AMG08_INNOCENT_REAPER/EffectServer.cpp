/*****************************************************************//**
 * @file   EffectServer.cpp
 * @brief  エフェクトサーバクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "EffectServer.h"
#include "EffectBase.h"

namespace inr {

  EffectServer::EffectServer(Game& game) : _game(game) {
    Clears();
  }

  EffectServer::~EffectServer() {
    Clears();
  }

  void EffectServer::Clears() {
    // 全コンテナの解放
    _effectsF.clear();
    _effectsB.clear();
    _addFormer.clear();
    _addBack.clear();
  }

  void EffectServer::Init() {
    // 各種初期化実行
    _update = false;
    _isAdd = false;
    _del = false;
    Clears();
  }

  void EffectServer::Process() {
    // 一時的格納用コンテナにエフェクトが登録されている場合、格納用コンテナへの登録処理へ移行
    if (_isAdd == true) MoveAddEffects();
    IsDelete();       // 消去処理の確認
    _update = true;   // 更新開始
    for (auto&& eff : _effectsB) eff->Process(); // 後景エフェクトの更新処理呼び出し
    for (auto&& eff : _effectsF) eff->Process(); // 前景エフェクトの更新処理呼び出し
    _update = false;  // 更新終了
  }

  void EffectServer::DrawBack() {
    for (auto&& eff : _effectsB) eff->Draw(); // 後景エフェクトの描画処理呼び出し
  }

  void EffectServer::DrawFormer() {
    for (auto&& eff : _effectsF) eff->Draw(); // 前景エフェクトの描画処理呼び出し
  }

  void EffectServer::Add(std::unique_ptr<EffectBase> eff, int type) {
    // コンテナを回しているかに応じて格納用コンテナと一時的格納コンテナのどちらに登録するか判定
    // true:処理を行っている場合は登録フラグを真にし、該当する一時的格納コンテナに登録
    // false:処理を行っていない場合は対応するコンテナに直で登録する
    switch (_update) {
    case true:
      _isAdd = true;
      switch (type) {
      case effect::type::FORMER:
        _addFormer.emplace_back(std::move(eff));
        return;
      case effect::type::BACK:
        _addBack.emplace_back(std::move(eff));
        return;
      default:
        return;
      }
      return;
    case false:
      switch (type) {
      case effect::type::FORMER:
        _effectsF.emplace_back(std::move(eff));
        return;
      case effect::type::BACK:
        _effectsB.emplace_back(std::move(eff));
        return;
      default:
        return;
      }
    }
  }

  void EffectServer::MoveAddEffects() {  
    // 要素がある場合は対応するコンテナに所有権を移行する
    if (_addFormer.empty() != true) {
      for(auto&& eff : _addFormer) _effectsF.emplace_back(std::move(eff));
      _addFormer.clear();
    }
    if (_addBack.empty() != true) {
      for(auto&& eff : _addBack)  _effectsB.emplace_back(std::move(eff));
      _addBack.clear();
    }
    _isAdd = false; // 登録完了
  }

  void EffectServer::IsDelete() {
    if (_del != true) return; // 条件を満たしていないため処理を中断
    std::vector<std::unique_ptr<EffectBase>> alives; // 避難用コンテナ
    // 消去フラグが立っていないエフェクトを一時的に避難させる
    for (auto&& eff : _effectsF) {
      if (eff->IsDel() == true) continue;
      alives.emplace_back(std::move(eff));
    }
    _effectsF.clear();
    // 再登録
    for (auto&& eff : alives) _effectsF.emplace_back(std::move(eff));
    alives.clear(); // コンテナ解放
    // 消去フラグが立っていないエフェクトを一時的に避難させる
    for (auto&& eff : _effectsB) {
      if (eff->IsDel() == true) continue;
      alives.emplace_back(std::move(eff));
    }
    _effectsB.clear();
    // 再登録
    for (auto&& eff : alives) _effectsB.emplace_back(std::move(eff));
    _del = false; // 消去完了
  }
}