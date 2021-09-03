#pragma once
#include "EnemyBase.h"

namespace inr {

	constexpr auto SOLDIER_IMAGE_W = 160;
	constexpr auto SOLDIER_IMAGE_H = 160;

	class SoldierDoll : public EnemyBase {
	public:
		SoldierDoll(Game& game);
		~SoldierDoll() override;

		void Init() override;
		void Process() override;

		bool Hit() override;

		// α版限定処理
		/*void SetStatus(Vector2 spawn, std::string soulcolor) override {
			_position = spawn;
			if (soulcolor == "red") {
				_sState = SoulState::RED;
				_aState = ActionState::IDOL;
				_divKey.first = enemy::red::SOLDIER_IDOL;
			}
			else if (soulcolor == "blue") {
				_sState = SoulState::BLUE;
				_aState = ActionState::IDOL;
				_divKey.first = enemy::blue::SOLDIER_IDOL;
			}
			else if (soulcolor == "empty") {
				_sState = SoulState::EMPTY;
				_aState = ActionState::EMPTY;
				_divKey.first = enemy::SOLDIER_EMPTY;
			}
		}*/
	private:
		int _aInterval;	// アクションインターバル
		int _stay;	// 待機カウンタ
		double _patrolX;
		bool _drawStop;	// 描画更新停止

		void Move() override;
		void Action() override;
		// void AnimationInit() override;

		void Patrol();	// 巡回
		void PatrolOn();	// 巡回状態に移行
		void Escape();
		void AttackOn() override;	// 攻撃開始
		void Attack();	// 攻撃処理

		void ChangeIdol();
		void PositionUpdate();
		void StateUpdate();	// 状態の切り替え

		// 干渉可能範囲の算出
		AABB VitalPart(Collision& col);
		AABB DamageBox();	// ダメージボックスの算出
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

