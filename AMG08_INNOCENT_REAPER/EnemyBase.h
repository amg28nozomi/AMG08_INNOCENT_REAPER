#pragma once
#include "ObjectBase.h"
#include "Collision.h"
// #include "Soul.h"

namespace inr {

	namespace enemy {
		// キー情報
		constexpr auto SOLDIER_EMPTY = "soldier_empty";	// 抜け殻

		namespace red {	// 魂（赤）
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";	// 巡回中
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";	// 攻撃
		}

		namespace blue { // 青の魂
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";	// 起き上がり
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";	// アイドル
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";	// 巡回中
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";	// 逃走
		}

	}

	class Soul; // まだ作ってないよ

	class EnemyBase : public ObjectBase {
	protected:
		// ソルジャードール、ビッグドール、クロウドール
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// 抜け殻、巡回、攻撃、逃避
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, ESCAPE
		};
		// 空、赤、青
		enum class SoulState{
			EMPTY, RED, BLUE
		};

		// Soul _soul;
		EnemyType _eType;
		ActionState _aState;
		SoulState _sState;

		AABB _searchBox; // 索敵用当たり判定
		boxs _collisions;	// 各種当たり判定格納用
		Vector2 _moveVector;	// 移動ベクトル
		int _aFrame;	// アクション実行のためのフレーム
		int _sounds;

		double _actionX;	// アクション実行時のx座標

		bool SearchPlayer(); // 索敵処理(自機の索敵

		virtual void Move() = 0; // 移動処理(派生先で定義)
		virtual void Action() = 0; // 行動(派生先で定義)
		// virtual void AnimationInit() = 0;
		// プレイヤーの現在座標から、左右どちらに居るかを割り出す
		virtual bool SearchPosition();
		virtual void Escape();	// 逃避
		virtual bool Hit();	// オブジェクトと接触したかどうか
	public:
		EnemyBase(Game& game);
		~EnemyBase() override;

		virtual void Init() override;
		void Draw() override;
		
		inline EnemyType GetEnemyType() { return _eType; }
		inline ActionState GetActionState() { return _aState; }
		inline SoulState GetSoulState() { return _sState; }

		// virtual void Process() override;
		// virtual void Draw() override;
	};
}

