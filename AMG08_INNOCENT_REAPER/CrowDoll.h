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
			constexpr auto CROW_RUSH = "crow_rash";	// 連撃
			constexpr auto CROW_BLINK = "crow_brink";	// 頭上からの強襲
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			constexpr auto CROW_DOWN = "crow_down";	// 倒れる
			// 画像サイズ
			constexpr auto CROW_SIZE = 500;	// 画像サイズ


			constexpr auto SE_VOICE = "crow_voice";	// 鳴き声
			constexpr auto SE_RUSH = "crow_rush";	// 連撃
		}
	}

	class CrowDoll : public EnemyBase {
	public:
		CrowDoll(Game& game);

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;
	private:
		enum class CrowState {
			// 死亡、待機、デバフ、咆哮(行動不能)、連続斬り、頭上からの強襲、伸びる爪、怯み、非活動状態
			DEATH, IDOL, DEBUF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _cState;	// クロウドールの状態
		Vector2 _target;	// 攻撃目標座標
		Vector2 _actionEnd;	// アクション終了
		int _life;	// 体力
		int _atkInterval;	// 攻撃の猶予時間
		int _pattern;	// 行動パターン

		int _actionCount;	// ラッシュカウント

		bool _setup;	// 準備は完了したか？
		bool _arm;	// 腕は生成したか？
		int _muteki;	// 無敵時間


		void ModeChange(CrowState nextState, std::string key);
		void GetTarget();	// 自機の座標を取得する
		bool SetState();	// 各種状態の管理
		int IsAnger();	// 起こり状態になっているか？
		bool IsVital();	// 現在のアニメーション中、魂は奪えるか？
		// bool Damage();	// 自身のダメージ処理を行う
		void Warp();	// 特定座標までワープする
		void WakeUp();	// 活動開

		void ChangeDirection();	// 向きの変更
		bool IsActive();	// 活動状態にあるか？
		bool IsBattle();

		void Move() override;	// 移動
		bool Floating();	// 浮遊処理
		void Debuf();	// デバフ処理
		void Rash();	// 連続攻撃
		void Blink();	// 落下攻撃
		void AddSoul();		// 魂を生み出す
		void Attack();	// 攻撃処理

		bool IsGravity();	// 重力処理を行うか？
		bool IsAttackEnd();	// 強制終了

		bool IsPlayerPos(double px);	// 自機は左右どちらにいるのか？

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

