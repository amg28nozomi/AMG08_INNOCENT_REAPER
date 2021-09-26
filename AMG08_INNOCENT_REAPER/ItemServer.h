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
		void ItemClear();	// �A�C�e���S����
		bool DelOn();	// �����t���O���I���ɂ���

		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;	// �I�u�W�F�N�g�̏������s����
		std::vector<std::shared_ptr<Item>> _items;	// �A�C�e���Ǘ��p

		bool Delete();
		bool IsCopy(std::shared_ptr<Item> item);
	};
}

