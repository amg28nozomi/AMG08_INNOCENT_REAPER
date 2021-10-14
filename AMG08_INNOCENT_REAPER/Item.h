/*****************************************************************//**
 * \file   Item.h
 * \brief  アイテムクラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include "ObjectValue.h"

namespace inr {
	namespace item {
		// アイテム
		constexpr auto ITEM = "item_light";		// キー
		constexpr auto ITEM_IMAGE = 150;		// 画像サイズ
	}
	// 二重インクルード防止
	class Game;
	// アイテムクラス
	class Item {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Item(Game& game);
		// デストラクタ
		~Item() = default;
		// 初期化
		void Init();
		// 更新
		void Process();
		// 描画
		void Draw();
		// オブジェクト情報の登録
		void SetParameter(ObjectValue ovalue);
		// オブジェクト情報の更新
		void ObjValueUpdate();
		// オブジェクトは既に入手されているか
		inline bool IsGet() { return _isGet; }
		// 消去フラグの取得
		inline bool IsDel() { return _del; }
		// オブジェクト情報の取得
		inline ObjectValue GetObjectValue() { return _oValue; }
	private:
		Game& _game;			// ゲーム参照
		ObjectValue _oValue;	// オブジェクト情報
		Collision _col;			// 当たり判定
		std::string _gkey;		// 画像キー
		Vector2 _position;		// 座標
		int _count;				// カウンタ
		int _maxFrame;			// アニメーションの描画フレーム数
		int _messageNo;			//　対応しているmessage番号
		bool _isGet;			// すでに入手されているか？
		bool _del;				// 消去フラグ
	};

}
