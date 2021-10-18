/*****************************************************************//**
 * @file   ItemServer.h
 * @brief  アイテムの管理を行うアイテムサーバクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	/** 二重インクルード防止 */
	class Item;
	/** アイテムサーバ */
	class ItemServer {
	public:
		/**
		 * @brief				コンストラクタ
		 */
		ItemServer();
		/**
		 * @brief				デストラクタ
		 */
		~ItemServer();
		/**
		 * @brief				更新処理
		 */
		void Process();
		/**
		 * @brief				描画処理
		 */
		void Draw();
		/**
		 * @brief				アイテムの登録
		 * @param item	登録するアイテムのシェアードポインタ
		 */
		void Add(std::shared_ptr<Item> item);
		/**
		 * @brief				アイテムの開放
		 */
		void ItemClear();
		/**
		 * @brief				消去フラグの起動
		 */
		bool DelOn();
		/**
		 * @brief				全アイテムの取得
		 * @return			アイテムを登録しているコンテナを返す
		 */
		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;																//!< オブジェクトの消去を行うか
		std::vector<std::shared_ptr<Item>> _items;	//!< アイテム管理用のコンテナ
		/**
		 * @brief				アイテムの消去
		 * @return			成功した場合はtrueを返す
		 *							失敗した場合はfalseを返す
		 */
		bool Delete();
	};
}

