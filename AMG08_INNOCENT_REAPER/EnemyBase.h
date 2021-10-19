/*****************************************************************//**
 * @file   EnemyBase.h
 * @brief  敵のスーパークラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"

namespace inr {
	/** 敵のコンパイル時定数 */
	namespace enemy {
		// 画像キー
		constexpr auto SOLDIER_EMPTY = "soldier_empty";					//!< 抜け殻(ソルジャードール)
		constexpr auto BIG_EMPTY = "big_empty";									//!< 抜け殻(ビッグドール)
		/** 赤い魂 */
		namespace red {
			// ソルジャードール
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";		//!< 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";			//!< 待機
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";		//!< 巡回中
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";		//!< 攻撃
			// ビッグドール 
			constexpr auto BIG_WAKEUP = "big_r_wakeup";						//!< 起き上がり
			constexpr auto BIG_IDOL = "big_r_stand";							//!< 待機
			constexpr auto BIG_PATROL = "big_r_patrol";						//!< 巡回
			constexpr auto BIG_TACKLE = "big_r_tackle";						//!< タックル
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";					//!< ヒップドロップ
			/** ビッグドールのコンパイル時定数 */
			namespace big {
				// 各種モーション画像の分割数
				constexpr auto WAKEUP_SIZE = 18;										//!< 起き上がりモーションの総分割数
				constexpr auto IDOL_SIZE = 11;											//!< 待機モーションの総分割数
				constexpr auto MOVE_SIZE = 11;											//!< 移動モーションの総分割数
				constexpr auto ATTACK_SIZE = 11;										//!< 攻撃モーションの総分割数
				constexpr auto HIPDROP_SIZE = 10;										//!< ヒップドロップモーションの総分割数
			}
		}
		/** 青い魂 */
		namespace blue {
			// ソルジャードールの画像キー
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";		//!< 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";			//!< 待機
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";		//!< 巡回中
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";		//!< 逃走
			// ビッグドールの画像キー
			constexpr auto BIG_WAKEUP = "big_b_wakeup";						//!< 起き上がり
			constexpr auto BIG_IDOL = "big_b_stand";							//!< 待機
			constexpr auto BIG_PATROL = "big_b_patrol";						//!< 巡回
			constexpr auto BIG_ESCAPE = "big_b_escape";						//!< 逃避
			/** ビッグドールのコンパイル時定数 */
			namespace big {
				// 各種モーション画像の総分割数
				constexpr auto WAKEUP_SIZE = 18;										//!< 起き上がりモーションの総分割数
				constexpr auto IDOL_SIZE = 11;											//!< 待機モーションの総分割数
				constexpr auto PATROL_SIZE = 11;										//!< 巡回モーションの総分割数
				constexpr auto ESCAPE_SIZE = 16;										//!< 逃避モーションの総分割数
			}
		}
		// 逃走時の移動量
		constexpr auto ESCAPE_MAX = 1000;												//!< 逃避時の総移動量
		constexpr auto ESCAPE_VECTOR = 120;											//!< 逃避時の移動量(60F)
		// 攻撃時の移動量
		constexpr auto ATTACK_MAX = 1000;												//!< 攻撃時の総移動量
		constexpr auto ATTACK_VECTOR = 180;											//!< 攻撃時の移動量(60F)
		// 向き
		constexpr auto MOVE_LEFT = true;												//!< 左向き
		constexpr auto MOVE_RIGHT = false;											//!< 右向き
	}
	/** 二重インクルード防止 */
	class SoulSkin;
	/** 敵のスーパークラス */
	class EnemyBase : public ObjectBase {
	protected:
		/**
		 * @brief	敵の種類を表す列挙型
		 *				ソルジャードール、ビッグドール、クロウドール
		 */
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		/**
		 * @brief	敵の状態を表す列挙型
		 *				抜け殻、待機、巡回、攻撃、ヒップドロップ、逃避、起き上がり
		 */
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, HIPDROP, ESCAPE, WAKEUP
		};
		EnemyType _eType;									//!< タイプ
		ActionState _aState;							//!< 状態
		boxs _collisions;									//!< 各種当たり判定格納用
		AABB _searchBox;									//!< 索敵用当たり判定
		Vector2 _moveVector;							//!< 移動ベクトル
		std::shared_ptr<SoulSkin> _soul;	//!< 魂
		int _aFrame;											//!< アクション実行のためのフレーム
		int _aInterval;										//!< アクションインターバル
		int _stay;												//!< 待機カウンタ
		double _actionX;									//!< アクション時のx総移動量
		double _patrolX;									//!< 巡回時のx総移動量
		bool _isAction;										//!< アクション状態に移行するかのフラグ
		bool _drawStop;										//!< 描画更新停止
		/**
		 * @brief								アニメーションカウンタの管理
		 */
		void AnimationCount() override;
		/**
		 * @brief								状態遷移処理
		 * @param nextstate			遷移する状態
		 * @param key						遷移後の再生画像キー
		 */
		void ChangeState(ActionState nextstate, std::string key);
		/**
		 * @brief								自機が索敵範囲内に入っているかの判定
		 * @return							範囲内にいる場合はtrueを返す
		 *											範囲外にいる場合はfalseを返す
		 */
		bool SearchPlayer();
		/**
		 * @brief								移動処理を行う純粋仮想関数(定義は派生先で行う)
		 */
		virtual void Move() = 0;
		/**
		 * @brief								アクション状態に移行するかの判定
		 */
		virtual void Action();
		/**
		 * @brief								自機が左右どちら側にいるかの判定
		 * @return							左側に居る場合はtrueを返す
		 *											右側に居る場合はfalseを返す
		 */
		virtual bool SearchPosition();
		/**
		 * @brief								巡回状態への遷移処理
		 */
		virtual void PatrolOn();
		/**
		 * @brief								逃避状態への遷移処理
		 */
		virtual void EscapeOn();
		/**
		 * @brief								攻撃状態への遷移処理
		 */
		virtual void AttackOn();
		/**
		 * @brief								接触しているマップチップに効果があるかの判定
		 * @return							効果がある場合はtrueを返す
		 *											効果がない場合はfalseを返す
		 */
		virtual bool IsStandChip();
		/**
		 * @brief								死亡処理
		 */
		virtual void Death();
		/**
		 * @brief								待機状態の調整
		 * @param stay					遷移後の待ち時間
		 */
		virtual void ChangeIdol(int stay);
		/**
		 * @brief								座標等の各種更新処理
		 */
		virtual void PositionUpdate();
		/**
		 * @brief								効果音の再生処理
		 * @param skey					効果音のキー
		 * @return							再生に成功した場合はtrueを返す
		 *											失敗した場合はfalseを返す
		 */
		bool PlaySe(const std::string skey);
		/**
		 * @brief								干渉可能範囲の取得
		 * @param col						算出に使用する当たり判定ボックス
		 * @param vital					干渉範囲(x)
		 * @return							算出した当たり判定ボックスを返す
		 */
		AABB VitalPart(Collision& col, int vital = 0);
		/**
		 * @brief								攻撃判定ボックスの算出・取得
		 * @param fix						判定範囲の修正値(x)
		 * @return							算出した当たり判定ボックスを返す
		 */
		AABB DamageBox(int fix = 0);
	public:
		/**
		 * @brief								コンストラクタ
		 * @param game					ゲームクラスの参照
		 */
		EnemyBase(Game& game);
		/**
		 * @brief								デストラクタ
		 */
		~EnemyBase() override;
		/**
		 * @brief								初期化処理
		 */
		virtual void Init() override;
		/**
		 * @brief								描画処理
		 */
		void Draw() override;
		/**
		 * @brief								自機アクションボックスとの衝突判定
		 * @param	ckey					対象の現在の状態(キー)
		 * @param	acollision		対象の当たり判定ボックス
		 * @param	direction			対象の向きフラグ
		 */
		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);
		/**
		 * @brief								オブジェクト情報の登録
		 * @param objValue			オブジェクト情報
		 */
		virtual void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief								抜け殻かどかの判定
		 * @return							抜け殻の場合はtrueを返す
		 *											抜け殻ではない場合はfalseを返す
		 */
		bool IsEmpty();
		/**
		 * @brief								保持している魂の所有権を手放し、活性状態に移行させる
		 * @return							成功した場合はtrueを返す
		 *											失敗した場合はfalseを返す
		 */
		bool SoulPop();
		/**
		 * @brief								このエネミーはボスかどうか	
		 * @return							falseを返す
		 */
		inline virtual bool IsBoss() { return false; }
		/**
		 * @brief								タイプの取得
		 * @return							タイプを返す
		 */
		inline EnemyType GetEnemyType() { return _eType; }
		/**
		 * @brief								状態の取得
		 * @return							現在の状態を返す
		 */
		inline ActionState GetActionState() { return _aState; }
		/**
		 * @brief								移動ベクトルの取得
		 * @return							移動ベクトルを返す
		 */
		inline Vector2 GetMoveVector() override { return _moveVector; }
		/**
		 * @brief								抜け殻時の当たり判定ボックスを取得
		 * @return							抜け殻時の当たり判定ボックスを返す
		 */
		AABB EmptyBox();
	};
}

