#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace crowdoll {
			// 各種モーションおよびキー
			constexpr auto CROW_ARM = "crow_arm";	// 腕
			constexpr auto CROW_IDOL = "crow_stand";	// 待機
			constexpr auto CROW_DEBUF = "crow_debuf";	// デバフモーション
			constexpr auto CROW_GROWARM = "crow_growarm";	// 伸びる爪
			constexpr auto CROW_RASH = "crow_rash";	// 連撃
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			// 画像サイズ
			constexpr auto CROW_SIZE = 380;	// 画像サイズ

		}
	}

	class CrowDoll : public EnemyBase {
	public:
		CrowDoll(Game& game);

		void Init() override;
		void Process() override;
		void Draw() override;

		void WakeUp();	// 活動開始
	private:
		enum class CrowState {
			// 死亡、待機、デバフ、咆哮(行動不能)、連続斬り、怯み、非活動状態
			DEATH, IDOL, DEBUF, ROAR, RASH, WINCE, SLEEP
		};
		CrowState _cState;	// クロウドールの状態
		Vector2 _target;	// 攻撃目標座標

		void ModeChange(CrowState nextState, std::string key);

	};
}

