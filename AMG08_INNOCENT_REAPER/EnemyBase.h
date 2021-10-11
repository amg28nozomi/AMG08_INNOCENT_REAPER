/*****************************************************************//**
 * \file   EnemyBase.h
 * \brief  敵のスーパークラス
 *
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Collision.h"
namespace inr {
	namespace enemy {
		// 画像キー(ResourceServer)
		constexpr auto SOLDIER_EMPTY = "soldier_empty";			// 抜け殻(ソルジャードール)
		constexpr auto BIG_EMPTY = "big_empty";					// 抜け殻(ビッグドール)
		// 赤い魂
		namespace red {
			// ソルジャードール
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";	// 巡回中
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";	// 攻撃
			// ビッグドール 
			constexpr auto BIG_WAKEUP = "big_r_wakeup";			// 起き上がり
			constexpr auto BIG_IDOL = "big_r_stand";			// 待機
			constexpr auto BIG_PATROL = "big_r_patrol";			// 巡回
			constexpr auto BIG_TACKLE = "big_r_tackle";			// 攻撃（タックル）
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";		// 攻撃（ヒップドロップ）

			namespace big {
				// 各種モーションの分割数
				constexpr auto WAKEUP_SIZE = 5;
				constexpr auto IDOL_SIZE = 3;
				constexpr auto MOVE_SIZE = 5;
				constexpr auto ATTACK_SIZE = 5;
				constexpr auto HIPDROP_SIZE = 7;
				/*constexpr auto WAKEUP_SIZE = 17;
				constexpr auto IDOL_SIZE = 11;
				constexpr auto MOVE_SIZE = 13;
				constexpr auto ATTACK_SIZE = 11;
				constexpr auto HIPDROP_SIZE = 11;*/
			}
		}
		// 青い魂
		namespace blue {
			// ソルジャードールの画像キー
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";	// 巡回中
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";	// 逃走
			// ビッグドールの画像キー
			constexpr auto BIG_WAKEUP = "big_b_wakeup";
			constexpr auto BIG_IDOL = "big_b_stand";
			constexpr auto BIG_PATROL = "big_b_patrol";
			constexpr auto BIG_ESCAPE = "big_b_escape";

			namespace big {
				// 各種モーションの総分割数
				constexpr auto WAKEUP_SIZE = 5;
				constexpr auto IDOL_SIZE = 3;
				constexpr auto PATROL_SIZE = 4;
				constexpr auto ESCAPE_SIZE = 5;
				/*constexpr auto WAKEUP_SIZE = 18;
				constexpr auto IDOL_SIZE = 11;
				constexpr auto PATROL_SIZE = 11;
				constexpr auto ESCAPE_SIZE = 13;*/
			}
		}
		// 逃走時の移動量
		constexpr auto ESCAPE_MAX = 1000;						// 逃避時の総移動量
		constexpr auto ESCAPE_VECTOR = 120;						// 逃避時の移動量(60F)
		// 攻撃時の移動量
		constexpr auto ATTACK_MAX = 1000;						// 攻撃時の総移動量
		constexpr auto ATTACK_VECTOR = 180;						// 攻撃時の移動量(60F)
		// 向き
		constexpr auto MOVE_LEFT = true;
		constexpr auto MOVE_RIGHT = false;
	}
	// 二重インクルード防止
	class SoulSkin;
	// 敵の基底クラス
	class EnemyBase : public ObjectBase {
	protected:
		// 敵の種類(列挙型)
		// ソルジャードール、ビッグドール、クロウドール
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// 敵の状態(列挙型)
		// 抜け殻、待機、巡回、攻撃、ヒップドロップ、逃避、起き上がり
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, HIPDROP, ESCAPE, WAKEUP
		};
		EnemyType _eType;					// 種類
		ActionState _aState;				// 状態
		boxs _collisions;					// 各種当たり判定格納用
		AABB _searchBox;					// 索敵用当たり判定
		Vector2 _moveVector;				// 移動ベクトル
		std::shared_ptr<SoulSkin> _soul;	// 魂
		int _aFrame;						// アクション実行のためのフレーム
		int _aInterval;						// アクションインターバル
		int _stay;							// 待機カウンタ
		double _actionX;					// アクション時のx総移動量
		double _patrolX;					// 巡回時のx総移動量
		bool _isSearch;						// プレイヤーを
		bool _isAction;						// アクション状態に移行するかのフラグ
		bool _drawStop;						// 描画更新停止
		// アニメーションカウンタの管理
		void AnimationCount() override;
		// 自機が索敵範囲内に入ったかの判定(true:範囲内にいる　false:いない)
		bool SearchPlayer();
		// 移動勝利(派生先で定義)
		virtual void Move() = 0;
		// アクション状態に移行するかの判定
		virtual void Action();
		// プレイヤーの現在座標から、左右どちらに居るかを割り出す
		virtual bool SearchPosition();
		// 巡回状態への遷移処理
		virtual void PatrolOn();
		// 逃避状態への遷移処理
		virtual void EscapeOn();
		// 攻撃状態への遷移処理
		virtual void AttackOn();
		// 立っているマップチップとの接触処理
		virtual bool IsStandChip();
		// 死亡処理
		virtual void Death();
		// 待機状態への調整(引数;遷移後の待ち時間)
		virtual void ChangeIdol(int stay);
		// 座標等の更新処理
		virtual void PositionUpdate();
		// 交換音の再生(引数:再生する効果音のキー)
		bool PlaySe(const std::string skey);
		// 状態遷移(引数1:遷移する状態　引数2:遷移後の再生画像)
		void ChangeState(ActionState nextstate, std::string key);
		// 干渉可能範囲の算出(引数1:基になる当たり判定　引数2:干渉可能な横幅)
		AABB VitalPart(Collision& col, int vital = 0);
		// 攻撃範囲の算出(引数:修正値)
		AABB DamageBox(int fix = 0);
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		EnemyBase(Game& game);
		// デストラクタ
		~EnemyBase() override;
		// 初期化
		virtual void Init() override;
		// 描画
		void Draw() override;
		// 種類の取得
		inline EnemyType GetEnemyType() { return _eType; }
		// 状態の取得
		inline ActionState GetActionState() { return _aState; }
		// 移動ベクトルの取得
		inline Vector2 GetMoveVector() override { return _moveVector; }
		// 自機アクションとの衝突判定
		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);
		// 抜け殻かどうかの判定
		bool IsEmpty();
		// ボスかどうかの判定
		inline bool IsBoss() { return _eType == EnemyType::CROW_DOLL; }
		// 保持している魂を生成する
		bool SoulPop();
		// オブジェクト情報の登録
		virtual void SetParameter(ObjectValue objValue) override;
		// 抜け殻時の当たり判定の取得
		AABB EmptyBox();
	};
}

