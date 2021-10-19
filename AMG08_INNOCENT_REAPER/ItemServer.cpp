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
		ItemClear();	// コンテナの解放
	}

	ItemServer::~ItemServer() {
		ItemClear();	// コンテナの解放
	}

	void ItemServer::ItemClear() {
		_isDel = false;
		_items.clear();
	}

	void ItemServer::Add(std::shared_ptr<Item> item) {
		_items.emplace_back(item);
	}

	void ItemServer::Process() {
		Delete();		// 消去判定
		for (auto item : _items) item->Process();	// アイテムの更新処理呼び出し
	}

	void ItemServer::Draw() {
		for (auto item : _items) item->Draw();		// アイテムの描画処理呼び出し
	}

	bool ItemServer::Delete() {
		if (_isDel == false) return false;	// 消去しない
		_isDel = false;											// フラグオフ
	}

	bool ItemServer::DelOn() {
		if (_isDel == true) return false;	// 既にフラグがオンの場合は処理を終了
		_isDel = true;										// 消去フラグ起動
		return true;
	}
}