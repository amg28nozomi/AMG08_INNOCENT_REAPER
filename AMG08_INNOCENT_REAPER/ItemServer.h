/*****************************************************************//**
 * \file   ItemServer.h
 * \brief  アイテムサーバクラス
 *		   アイテムの管理を行う
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	// 二重インクルード防止
	class Item;
	// アイテムサーバ
	class ItemServer {
	public:
		// コンストラクタ
		ItemServer();
		// デストラクタ
		~ItemServer();
		// 描画
		void Process();
		// 更新
		void Draw();
		// アイテムの登録
		void Add(std::shared_ptr<Item> item);
		// コンテナの解放
		void ItemClear();
		// 消去フラグの起動
		bool DelOn();
		// アイテムコンテナの取得
		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;								// オブジェクトの消去を行うか
		std::vector<std::shared_ptr<Item>> _items;	// アイテム管理用のコンテナ
		// アイテムの消去
		bool Delete();
	};
}

