#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace crowdoll {
			// 各種モーションおよびキー
			constexpr auto CROW_ARM = "crow_arm";	// 腕
			constexpr auto CROW_IDOL = "crow_stand";	// 待機
			constexpr auto CROW_DEBUF = "crow_debuf";	// デバフモーション
			constexpr auto CROW_GROWARM = "crow_growarm";	
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

		void Process() override;
		void Draw() override;
	private:

	};
}

