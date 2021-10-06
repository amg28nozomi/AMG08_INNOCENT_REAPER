#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		constexpr auto BIG_IMAGE = 280;
		// constexpr auto BIG_IMAGE = 340;

		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;

		// SE用キー
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";	// 倒れた場合
			constexpr auto SE_ESCAPE_VOICE = "big_escape";
			constexpr auto SE_TACKLE = "big_tackle";
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";

		}
	}

	// ビッグドール(敵)
	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();

		void Init() override;
		void Process() override;

		void SetParameter(ObjectValue objValue);	// オブジェクト情報の登録
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;	// 自機アクションとの衝突判定
	private:
		double _atkVec;	// 移動量
		int _moveCount;	// 移動カウント

		void HipDrop();	// ヒップドロップ
		void StateUpdate();	// 状態に応じた処理を行う

		void PatrolOn() override;	// 索敵処理判定オン
		void AttackOn() override;	// 攻撃処理判定オン
		void EscapeOn() override;	// 逃走処理判定オン
		void Action() override;	// アクション状態に移行するか
		void Death() override;	// 死亡処理
		void ChangeIdol(int stay) override;	// 待機状態への遷移
		void Move();	// 移動処理
		void PositionUpdate() override;	// 座標更新
		void Attack();	// 攻撃

		AABB NowCollision(std::string key) override;	// 現在の当たり判定の取得
	};
}

