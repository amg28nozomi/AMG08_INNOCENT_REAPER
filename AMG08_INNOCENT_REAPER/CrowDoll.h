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
			constexpr auto CROW_BLINK = "crow_brink";	// 頭上からの強襲
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

		void WakeUp();	// 活動開

		void SetParameter(ObjectValue objValue) override;
	private:
		enum class CrowState {
			// 死亡、待機、デバフ、咆哮(行動不能)、連続斬り、頭上からの強襲、怯み、非活動状態
			DEATH, IDOL, DEBUF, ROAR, RASH, BLINK, WINCE, SLEEP
		};
		CrowState _cState;	// クロウドールの状態
		Vector2 _target;	// 攻撃目標座標
		int _life;	// 体力
		int _atkInterval;	// 攻撃の猶予時間

		void ModeChange(CrowState nextState, std::string key);
		void GetTarget();	// 自機の座標を取得する
		bool SetState();	// 各種状態の管理
		bool IsAnger();	// 起こり状態になっているか？
		bool IsVital();	// 現在のアニメーション中、魂は奪えるか？
		bool Damage();	// 自身のダメージ処理を行う

		void Debuf();	// デバフ処理
		void AddSoul();		// 魂を生み出す

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

