/*****************************************************************//**
 * @file   ItemServer.cpp
 * @brief  アイテムの管理を行うアイテムサーバクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ItemServer.h"
#include "Item.h"
#include <algorithm>

namespace inr {

  ItemServer::ItemServer() {
    ItemClear();
  }

  ItemServer::~ItemServer() {
    ItemClear();
  }

  void ItemServer::ItemClear() {
    _isDel = false;
    _items.clear();
  }

  void ItemServer::Add(std::shared_ptr<Item> item) {
    _items.emplace_back(item);
  }

  void ItemServer::Process() {
    Delete(); // 消去判定
    for (auto item : _items) item->Process();
  }

  void ItemServer::Draw() {
    for (auto item : _items) item->Draw();
  }

  bool ItemServer::Delete() {
    if (_isDel == false) return false; // 消去しない
    _isDel = false;
  }

  bool ItemServer::DelOn() {
    if (_isDel == true) return false; // 既にフラグがオンの場合は処理を終了
    _isDel = true;
    return true;
  }
}