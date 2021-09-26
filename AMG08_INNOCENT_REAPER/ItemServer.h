#pragma once
#include <memory>
#include <vector>

namespace inr {

	class Item;

	class ItemServer {
	public:
		ItemServer();
		~ItemServer();

		void Process();
		void Draw();

		void Add(std::shared_ptr<Item> item);
		void ItemClear();	// アイテム全消去
		bool DelOn();	// 消去フラグをオンにする

		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;	// オブジェクトの消去を行うか
		std::vector<std::shared_ptr<Item>> _items;	// アイテム管理用

		bool Delete();
		bool IsCopy(std::shared_ptr<Item> item);
	};
}

