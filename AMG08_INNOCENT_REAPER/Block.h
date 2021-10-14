/*****************************************************************//**
 * @file   Block.h
 * @brief  壊れる岩クラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	/** ギミックのコンパイル時定数 */
	namespace gimmick {
		/** 壊れる岩のコンパイル時定数 */
		namespace block {
			constexpr auto BRAKE_ON = true;		//!< 破壊された
			constexpr auto BRAKE_OFF = false;	//!< 破壊されていない
		}
	}
	/** 壊れる岩ギミック */
	class Block : public GimmickBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param	game	ゲームクラスの参照
		 */
		Block(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Block() = default;
		/**
		 * @brief	更新処理
		 */
		void Process() override;
		/**
		 * @brief	描画処理
		 */
		void Draw() override;
		/**
		 * @brief								オブジェクト情報の登録
		 * @param	objectValue		オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief	オブジェクト情報の更新
		 */
		void ObjValueUpdate() override;
		/**
		 * @brief		オブジェクトは壊れているか？
		 * @return	壊れている場合はtrueを返す
		 * @return	壊れていない場合はfalseを返す
		 */
		inline bool IsBreak() { return _break; }
		/**
		 * @brief						オブジェクトの押し出し処理
		 * @param	box				対象の当たり判定ボックス
		 * @param	pos				対象の座標ベクトル(参照)
		 * @param	move			対象の移動ベクトル(参照)
		 * @param	direction	対象の向きフラグ
		 * @return					衝突している場合はtrueを返す
		 * @return					衝突していない場合はfalseを返す
		 */
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction);
		/**
		 * @brief	破壊処理
		 */
		void Break();
	private:
		int _pal;			//!< 透明度
		bool _break;	//!< 壊されたか否か
	};
}

