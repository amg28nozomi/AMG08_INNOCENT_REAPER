#include "ItemServer.h"
#include "Item.h"
#include <algorithm>
// #include <iterator>

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
		Delete();
		for (auto item : _items) item->Process();
	}

	void ItemServer::Draw() {
		for (auto item : _items) item->Draw();
	}

	bool ItemServer::Delete() {
		if (_isDel == false) return false;

		// 消去フラグがオンになっているオブジェクトのみを取り除く
		/*_items = std::copy_if(_items.begin(), _items.end(), _items.begin(), [](Item* i) {return i->IsDel() != false; });
		_items.clear();*/
		_isDel = false;
	}

	bool ItemServer::DelOn() {
		if (_isDel == true) return false;	// 既にフラグがオンの場合は処理を終了
		_isDel = true;	// 消去フラグをオンにする
		return true;
	}

	bool ItemServer::IsCopy(std::shared_ptr<Item> item) {
		return item->IsDel() != true;
	}
}