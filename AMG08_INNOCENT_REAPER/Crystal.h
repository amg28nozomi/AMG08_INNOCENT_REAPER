/*****************************************************************//**
 * @file   Crystal.h
 * @brief  水晶クラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"
#include "SoulSkin.h"
#include <memory>
#include <vector>

namespace inr {
	/** 多重インクルード防止 */
	class Door;
	/** 水晶ギミック */
	class Crystal : public GimmickBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Crystal(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Crystal() = default;
		/**
		 * @brief	描画処理
		 */
		void Draw() override;
		/**
		 * @brief						オブジェクト情報の登録
		 * @param objValue	オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief オブジェクト情報の更新
		 */
		void ObjValueUpdate() override;
		/**
		 * @brief							自機アクションボックスとの衝突判定
		 * @param	ckey				対象の現在の状態(キー)
		 * @param	acollision	対象の当たり判定ボックス
		 * @param	direction		対象の向きフラグ
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction);
	private:
		std::shared_ptr<SoulSkin> _soul;						//!< 保持している魂
		std::vector<std::shared_ptr<Door>> _doors;	//!< 保持しているドア
		/**
		 * @brief			ドアの設定
		 * @param key	扉の画像キーが格納されたコンテナ
		 */
		void SetDoors(std::vector<std::string> key);
		/**
		 * @brief	画像キーの切り替え
		 */
		void GraphKey();
		/**
		 * @brief	魂の生成処理
		 */
		void AddSoul();
		/**
		 * @brief				ドアと現在保有している魂の色は一致しているか
		 * @param type	ドアの色
		 * @return			一致している場合はtrueを返す
		 *							不一致の場合はfalseを返す
		 */
		bool IsOpen(int type);
	};
}

