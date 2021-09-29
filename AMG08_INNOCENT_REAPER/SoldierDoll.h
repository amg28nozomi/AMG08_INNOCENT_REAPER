#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace soldier {
			constexpr auto ATTACK_SE = "sd_attack_se";	// Ø‚è‚Â‚¯
			constexpr auto ATTACK_VOICE = "sd_attack_voice";	// Ø‚è•t‚¯º
			constexpr auto DOWN = "sd_down";	// “|‚ê‚½Û‚ÌSE
			constexpr auto ESCAPE_SE = "sd_escape_se";
			constexpr auto ESCAPE_VOICE = "sd_escape_voice";
			constexpr auto MOVE = "sd_move";
		}
	}

	constexpr auto SOLDIER_IMAGE_W = 160;
	constexpr auto SOLDIER_IMAGE_H = 160;

	class SoldierDoll : public EnemyBase {
	public:
		SoldierDoll(Game& game);
		~SoldierDoll() override;

		void Init() override;
		void Process() override;

		bool Hit() override;

		void SetParameter(ObjectValue objValue) override;
	private:

		void Move() override;

		void Patrol();	// „‰ñ
		void PatrolOn() override;	// „‰ñó‘Ô‚ÉˆÚs
		void Escape();
		void EscapeOn() override;
		void AttackOn() override;	// UŒ‚ŠJn
		void Attack();	// UŒ‚ˆ—

		void ChangeIdol(int stay) override;
		void PositionUpdate() override;
		void StateUpdate();	// ó‘Ô‚ÌØ‚è‘Ö‚¦

		void Action() override;

		void Death() override;	// €–Sˆ—
		bool StopSound();

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

