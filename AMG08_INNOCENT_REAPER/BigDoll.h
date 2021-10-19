/*****************************************************************//**
 * @file   BigDoll.h
 * @brief  ビッグドールクラス（エネミーベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	/** 敵のコンパイル時定数 */
	namespace enemy {
		/** ビッグドールの画像サイズ */
		constexpr auto BIG_IMAGE = 340;
		/** 当たり判定(横幅) */
		constexpr auto BIG_WIDTH = 160;
		/** 当たり判定(高さ) */
		constexpr auto BIG_HEIGHT = 200;
		/** ビッグドールのコンパイル時定数 */
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";									//!< ダウン時SE
			constexpr auto SE_ESCAPE_VOICE = "big_escape";				//!< 逃走時ボイス
			constexpr auto SE_TACKLE = "big_tackle";							//!< タックルSE
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";	//!< タックル時ボイス
		}
	}
	/** ビッグドール(敵) */
	class BigDoll : public EnemyBase {
	public:
		/**
		 * @brief							コンストラクタ
		 * @param	game				ゲームクラスの参照
		 */
		BigDoll(Game& game);
		/**
		 * @brief							デストラクタ
		 */
		~BigDoll();
		/**
		 * @brief							初期化処理
		 */
		void Init() override;
		/**
		 * @brief							更新処理
		 */
		void Process() override;
		/**
		 * @brief							オブジェクト情報の登録
		 * @param	objValue		オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue);
		/**
		 * @brief	自機アクションボックスとの衝突判定
		 * @param	ckey				対象の現在の状態(キー)
		 * @param	acollision	対象の当たり判定ボックス
		 * @param	direction		対象の向きフラグ
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	private:
		int _moveCount;				//!< 移動カウント
		double _atkVec;				//!< 移動量
		/**
		 * @brief							ヒップドロップ
		 */
		void HipDrop();
		/**
		 * @brief							状態に応じた処理の更新
		 */
		void StateUpdate();
		/**
		 * @brief							巡回処理判定オン
		 */
		void PatrolOn() override;
		/**
		 * @brief							攻撃処理判定オン
		 */
		void AttackOn() override;
		/**
		 * @brief							逃走処理判定オン
		 */
		void EscapeOn() override;
		/**
		 * @brief							アクション状態に移行するか
		 */
		void Action() override;
		/**
		 * @brief							死亡処理
		 */
		void Death() override;
		/**
		 * @brief							待機状態の設定
		 * @param	stay				待機時間
		 */
		void ChangeIdol(int stay) override;
		/**
		 * @brief							移動処理
		 */
		void Move();
		/**
		 * @brief							座標更新
		 */
		void PositionUpdate() override;
		/**
		 * @brief							攻撃処理
		 */
		void Attack();
		/**
		 * @brief							現在の当たり判定の取得
		 * @param	key					現在の状態を表すキー
		 * @return						抜け殻ではない場合は通常の当たり判定を返す
		 *										抜け殻の場合は抜け殻用の当たり判定を返す
		 */
		AABB NowCollision(std::string key) override;
	};
}

