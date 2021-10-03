#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace crowdoll {
			// 各種モーションおよびキー
			constexpr auto CROW_ARM = "crow_arm";	// 腕
			constexpr auto CROW_IDOL = "crow_stand";	// 待機
			constexpr auto CROW_DEBUFF = "crow_debuf";	// デバフモーション
			constexpr auto CROW_GROWARM = "crow_growarm";	// 伸びる爪
			constexpr auto CROW_RUSH = "crow_rash";	// 連撃
			constexpr auto CROW_BLINK = "crow_brink";	// 頭上からの強襲
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			constexpr auto CROW_DOWN = "crow_down";	// 倒れる
			// 画像サイズ
			constexpr auto CROW_SIZE = 500;	// 画像サイズ


			constexpr auto SE_VOICE = "crow_voice";	// 鳴き声
			constexpr auto SE_ROAR = "crow_roar_voice";	// 咆哮ボイス
			constexpr auto SE_RUSH = "crow_rush";	// 連撃
			constexpr auto SE_GROWARM = "crow_growarm";	// 地中攻撃
			constexpr auto SE_BLINK_ATTACK = "crow_blink_attack";
			constexpr auto SE_DEBUF = "crow_debuf";	// デバフボイス
			constexpr auto SE_DEATH = "crow_death";	// 死亡SE

			namespace motion {
				constexpr auto IDOL = 13;
				constexpr auto RUSH = 35;
				constexpr auto BLINK = 13;
				constexpr auto GROWARM = 10;
				constexpr auto ROAR = 11;
				constexpr auto DEBUF = 22;
				constexpr auto WINCE = 7;
				constexpr auto DOWN = 26;
			}
		}
	}

	namespace effect {
		namespace crow {
			constexpr auto ROAR = "cd_roar_effect";	// 咆哮
			constexpr auto ARM = "cd_arms_effect";	// 腕
			constexpr auto RUSH = "cd_rush_effect";	// 連続切り
			constexpr auto GROW = "cd_grow_chage_effect"; // 地中刺し溜め
			constexpr auto BLINK = "cd_blink_effect";	// 転移
			constexpr auto BLINK_ATTACK = "cd_blink_attack_effect";	// 落下攻撃
			constexpr auto DEBUF = "cd_debuf_effect";	// デバフエフェクト
			constexpr auto AURA = "cd_aura_effect";
			constexpr auto DEATH = "cd_death_effect";	// 死亡エフェクト

			constexpr auto ROAR_WIDTH = 3840;
			constexpr auto ROAR_HEIGHT = 2160;

			constexpr auto ARM_WIDTH = 750;
			constexpr auto ARM_HEIGHT = 600;

			constexpr auto RUSH_WIDTH = 800;
			constexpr auto RUSH_HEIGHT = 600;

			constexpr auto GROW_IMAGE = 400;
			constexpr auto BLINK_IMAGE = 800;

			constexpr auto BLINK_ATTACK_W = 560;
			constexpr auto BLINK_ATTACK_H = 800;
			constexpr auto BLINL_ATTACK_MAX = 10;

			constexpr auto AURA_IMAGE = 500;
			constexpr auto AURA_MAX = 30;

			constexpr auto DEBUF_MAX = 10;

			constexpr auto DEATH_IMAGE = 600;
			constexpr auto DEATH_MAX = 14;

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
		Vector2 _warpPos;	// 転移後の座標
		int _life;	// 体力
		int _atkInterval;	// 攻撃の猶予時間
		int _pattern;	// 行動パターン
		int _debuffCount;	// デバフを発動するか？

		int _actionCount;	// ラッシュカウント
		int _debuffChage;	// デバフ溜め

		bool _setup;	// 準備は完了したか？
		bool _arm;	// 腕は生成したか？
		bool _debuf;	// デバフフラグ
		bool _changeState;	// 状態を切り替えるか？
		bool _isAnimation;	// モーション再生を行うか
		bool _isWarp;	// ワープを行うか？
		bool _isAnger;	// 怒りフラグ
		bool _wait;	// 待機フラグ
		int _muteki;	// 無敵時間



		void ModeChange(CrowState nextState, std::string key);
		void GetTarget();	// 自機の座標を取得する
		bool SetState();	// 各種状態の管理
		int IsAnger();	// 起こり状態になっているか？
		bool IsVital();	// 現在のアニメーション中、魂は奪えるか？
		// bool Damage();	// 自身のダメージ処理を行う

		void WarpOn();	// 転移セット

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
		bool AngerOn();	// 怒り状態に遷移する

		bool IsPlayerPos(double px);	// 自機は左右どちらにいるのか？
		bool IsDead() override;
		bool DeathOn();	// 死亡処理の起動
		bool DollsEnd();

		AABB NowCollision(std::string key) override;
		std::pair<AABB, AABB> BlinkVitalPart(Collision& col, int vital);

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;

		// 引数1:生成地点、引数2:追従処理を行うか？(true:追従処理有り, false:追従処理無し)
		bool AddWarpEffect(Vector2 spwan, bool target = false);
		bool AddRushEffect();
		bool AddBlinkEffect();	// 攻撃エフェクト
		bool AddSmokeEffect();
		bool AddAngerEffect();	// 怒りエフェクト
		bool AddDebufEffect();
		bool IsPlayerPosition();	// 自機は左右どちらにいるのか

		bool AttackBox(bool flag);
	};
}

