#pragma once
#include "ObjectBase.h"
#include "Collision.h"
// #include "Soul.h"

namespace inr {

	namespace enemy {
		// キー情報
		constexpr auto SOLDIER_EMPTY = "soldier_empty";	// 抜け殻
		constexpr auto BIG_EMPTY = "big_empty";	// 抜け殻(ビッグドール)

		namespace red {	// 魂（赤）
			/* ソルジャードール */
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";	// 巡回中
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";	// 攻撃

			/* ビッグドール */
			constexpr auto BIG_WAKEUP = "big_r_wakeup";		// 起き上がり
			constexpr auto BIG_IDOL = "big_r_stand";		// 待機
			constexpr auto BIG_PATROL = "big_r_patrol";		// 巡回
			constexpr auto BIG_TACKLE = "big_r_tackle";		// 攻撃（タックル）
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";	// 攻撃（ヒップドロップ）
		}

		namespace blue { // 青の魂
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";	// 巡回中
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";	// 逃走

			constexpr auto BIG_WAKEUP = "big_b_wakeup";
			constexpr auto BIG_IDOL = "big_b_stand";
			constexpr auto BIG_PATROL = "big_b_patrol";
			constexpr auto BIG_ESCAPE = "big_b_escape";
		}

		constexpr auto ESCAPE_MAX = 1000;
		constexpr auto ESCAPE_VECTOR = 120;	// 1秒間(60frame)で移動する距離S

		constexpr auto ATTACK_MAX = 1000;
		constexpr auto ATTACK_VECTOR = 120;

		constexpr auto MOVE_LEFT = true;
		constexpr auto MOVE_RIGHT = false;
	}

	class SoulSkin;

	class EnemyBase : public ObjectBase {
	protected:
		// ソルジャードール、ビッグドール、クロウドール
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// 抜け殻、巡回、攻撃、逃避、起き上がり
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, ESCAPE, WAKEUP
		};

		// Soul _soul;
		EnemyType _eType;
		ActionState _aState;

		std::shared_ptr<SoulSkin> _soul;

		AABB _searchBox; // 索敵用当たり判定
		boxs _collisions;	// 各種当たり判定格納用
		Vector2 _moveVector;	// 移動ベクトル
		int _aFrame;	// アクション実行のためのフレーム
		int _sounds;

		int _aInterval;	// アクションインターバル
		int _stay;	// 待機カウンタ

		double _actionX;	// アクション実行時のx座標
		double _patrolX;
		bool _isSearch;	// プレイヤーを
		bool _drawStop;	// 描画更新停止

		void AnimationCount() override;

		bool SearchPlayer(); // 索敵処理(自機の索敵

		virtual void Move() = 0; // 移動処理(派生先で定義)
		virtual void Action();
		// virtual void AnimationInit() = 0;
		// プレイヤーの現在座標から、左右どちらに居るかを割り出す
		virtual bool SearchPosition();
		virtual void PatrolOn();	// 巡回
		virtual void EscapeOn();	// 逃避
		virtual void AttackOn();	// 攻撃開始
		virtual bool Hit();	// オブジェクトと接触したかどうか
		virtual bool IsStandChip();
		virtual void Death();
		virtual void ChangeIdol();

		void ChangeState(ActionState nextstate, std::string key);
		// 干渉可能範囲の算出
		AABB VitalPart(Collision& col, int vital = 0);
		AABB DamageBox(int fix = 0);	// ダメージボックスの算出

	public:
		EnemyBase(Game& game);
		~EnemyBase() override;

		virtual void Init() override;
		void Draw() override;
		
		inline EnemyType GetEnemyType() { return _eType; }
		inline ActionState GetActionState() { return _aState; }
		inline Vector2 GetMoveVector() override { return _moveVector; }

		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);

		bool IsEmpty();

		virtual void SetParameter(ObjectValue objValue) override;	// parameter

		// virtual void Process() override;
		// virtual void Draw() override;
	};
}

