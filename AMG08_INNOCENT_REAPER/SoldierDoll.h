#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace soldier {
			constexpr auto ATTACK_SE = "sd_attack_se";	// 切りつけ
			constexpr auto ATTACK_VOICE = "sd_attack_voice";	// 切り付け声
			constexpr auto DOWN = "sd_down";	// 倒れた際のSE
			constexpr auto ESCAPE_SE = "sd_escape_se";
			constexpr auto ESCAPE_VOICE = "sd_escape_voice";
			constexpr auto MOVE = "sd_move";
		}
	}

	constexpr auto SOLDIER_IMAGE_W = 160;
	constexpr auto SOLDIER_IMAGE_H = 160;

	class SoldierDoll : public EnemyBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		SoldierDoll(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~SoldierDoll() override;

		void Init() override;
		void Process() override;


		void SetParameter(ObjectValue objValue) override;
	private:

		void Move() override;

		void Patrol();	// 巡回
		void PatrolOn() override;	// 巡回状態に移行
		void Escape();
		void EscapeOn() override;
		void AttackOn() override;	// 攻撃開始
		void Attack();	// 攻撃処理

		void ChangeIdol(int stay) override;
		void PositionUpdate() override;
		void StateUpdate();	// 状態の切り替え

		void Action() override;

		void Death() override;	// 死亡処理
		bool StopSound();

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

