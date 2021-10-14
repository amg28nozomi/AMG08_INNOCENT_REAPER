/*****************************************************************//**
 * @file   CrowDoll.h
 * @brief  クロウドールクラス（エネミーベースクラスのサブクラス）
 * 本作のボスエネミー
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	/** 敵のコンパイル時定数 */
	namespace enemy {
		/** クロウドールのコンパイル時定数 */
		namespace crowdoll {
			// 各種モーションおよびキー
			constexpr auto CROW_ARM = "crow_arm";									//!< 腕
			constexpr auto CROW_IDOL = "crow_stand";							//!< 待機
			constexpr auto CROW_DEBUFF = "crow_debuf";						//!< デバフモーション
			constexpr auto CROW_GROWARM = "crow_growarm";					//!< 伸びる爪
			constexpr auto CROW_RUSH = "crow_rash";								//!< 連撃
			constexpr auto CROW_BLINK = "crow_brink";							//!< 頭上からの強襲
			constexpr auto CROW_ROAR = "crow_roar";								//!< 咆哮
			constexpr auto CROW_WINCE = "crow_wince";							//!< 怯み
			constexpr auto CROW_DOWN = "crow_down";								//!< 倒れる
			// 画像サイズ
			constexpr auto CROW_SIZE = 500;												//!< 分割画像1枚あたりの画像サイズ
			// SEキー（SoundServer）
			constexpr auto SE_VOICE = "crow_voice";								//!< 鳴き声(SE)
			constexpr auto SE_ROAR = "crow_roar_voice";						//!< 咆哮ボイス(SE)
			constexpr auto SE_RUSH = "crow_rush";									//!< 連撃攻撃(SE)
			constexpr auto SE_GROWARM = "crow_growarm";						//!< 地中攻撃(SE)
			constexpr auto SE_BLINK_ATTACK = "crow_blink_attack";	//!< 落下攻撃(SE)
			constexpr auto SE_DEBUF = "crow_debuf";								//!< デバフボイス(SE)
			constexpr auto SE_DEATH = "crow_death";								//!< 死亡(SE)
			/** モーション */
			namespace motion {
				constexpr auto IDOL = 13;														//!< 待機モーションの総分割数
				constexpr auto RUSH = 35;														//!< 連続攻撃モーションの総分割数
				constexpr auto BLINK = 13;													//!< 落下攻撃モーションの総分割数
				constexpr auto GROWARM = 10;												//!< 地中攻撃モーションの総分割数
				constexpr auto ROAR = 11;														//!< 咆哮モーションの総分割数
				constexpr auto DEBUF = 22;													//!< デバフモーションの総分割数
				constexpr auto WINCE = 7;														//!< 怯みモーションの総分割数
				constexpr auto DOWN = 26;														//!< ダウンモーションの総分割数
			}
		}
	}
	/** エフェクト */
	namespace effect {
		/** クロウドールエフェクト */
		namespace crow {
			// エフェクトの画像キー(ResourceServer)
			constexpr auto ROAR = "cd_roar_effect";									//!< 咆哮エフェクト
			constexpr auto ARM = "cd_arms_effect";									//!< 腕エフェクト
			constexpr auto RUSH = "cd_rush_effect";									//!< 連続切りエフェクト
			constexpr auto GROW = "cd_grow_chage_effect";						//!< 地中刺し溜めエフェクト
			constexpr auto BLINK = "cd_blink_effect";								//!< ワープエフェクト
			constexpr auto BLINK_ATTACK = "cd_blink_attack_effect";	//!< 落下攻撃エフェクト
			constexpr auto DEBUF = "cd_debuf_effect";								//!< デバフエフェクト
			constexpr auto AURA = "cd_aura_effect";									//!< 怒りエフェクト(オーラ)
			constexpr auto DEATH = "cd_death_effect";								//!< 死亡エフェクト
			// 咆哮エフェクト
			constexpr auto ROAR_WIDTH = 3840;												//!< 咆哮エフェクトの幅
			constexpr auto ROAR_HEIGHT = 2160;											//!< 咆哮エフェクトの高さ
			// 腕エフェクト
			constexpr auto ARM_WIDTH = 750;													//!< 腕エフェクトの幅
			constexpr auto ARM_HEIGHT = 600;												//!< 腕エフェクトの高さ
			// 連続攻撃エフェクト
			constexpr auto RUSH_WIDTH = 800;												//!< 連続攻撃エフェクトの幅										
			constexpr auto RUSH_HEIGHT = 600;												//!< 連続攻撃エフェクトの高さ
			// ワープエフェクト
			constexpr auto BLINK_IMAGE = 800;												//!< ワープエフェクトのサイズ
			// 落下攻撃エフェクト
			constexpr auto BLINK_ATTACK_W = 560;										//!< 落下攻撃エフェクトの幅
			constexpr auto BLINK_ATTACK_H = 800;										//!< 落下攻撃エフェクトの高さ
			constexpr auto BLINL_ATTACK_MAX = 10;										//!< 落下攻撃エフェクトの総分割数
			// オーラエフェクト
			constexpr auto AURA_IMAGE = 500;												//!< オーラエフェクトのサイズ
			constexpr auto AURA_MAX = 30;														//!< オーラエフェクトの総分割数
			// デバフエフェクトの
			constexpr auto DEBUF_MAX = 10;													//!< デバフエフェクトの総分割数
			// 死亡エフェクト
			constexpr auto DEATH_IMAGE = 600;												//!< 死亡エフェクトのサイズ
			constexpr auto DEATH_MAX = 14;													//!< 死亡エフェクトの総分割数
		}
	}
	/** クロウドール(ボスエネミー) */
	class CrowDoll : public EnemyBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		CrowDoll(Game& game);
		/**
		 * @brief	初期化処理
		 */
		void Init() override;
		/**
		 * @brief	更新処理
		 */
		void Process() override;
		/**
		 * @brief	描画処理
		 */
		void Draw() override;
		/**
		 * @brief						オブジェクト情報の登録
		 * @param objValue	オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue) override;
	private:
		/**
		 * @enmu	クロウドールの状態を表す列挙型
		 */
		enum class CrowState {
			// 死亡、待機、デバフ、咆哮(行動不能)、連続斬り、頭上からの強襲、伸びる爪、怯み、非活動状態
			DEATH, IDOL, DEBUFF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _crowState;	// クロウドールの状態
		Vector2 _target;		// 攻撃目標座標
		Vector2 _actionEnd;		// アクション終了
		Vector2 _warpPos;		// 転移後の座標
		int _life;				// 体力
		int _atkInterval;		// 攻撃の猶予時間
		int _pattern;			// 行動パターン
		int _debuffCount;		// デバフを発動するか？
		int _invincible;		// 無敵時間
		int _actionCount;		// ラッシュカウント
		int _debuffChage;		// デバフ溜め
		bool _setup;			// 準備は完了したか？
		bool _arm;				// 腕は生成したか？
		bool _debuf;			// デバフフラグ
		bool _changeState;		// 状態を切り替えるか？
		bool _isAnimation;		// モーション再生を行うか
		bool _isWarp;			// ワープを行うか？
		bool _isAnger;			// 怒りフラグ
		bool _wait;				// 待機フラグ
		// 状態遷移
		void ModeChange(CrowState nextState, std::string key);
		// 自機の座標を取得する
		void GetTarget();
		// 各種状態の管理
		bool SetState();
		// 怒り状態に突入しているかの判定
		int IsAnger();
		// 魂を奪える状態かの判定
		bool IsVital();
		// ワープ処理の起動
		void WarpOn();
		// ワープ処理
		void Warp();
		// 活動開始
		void WakeUp();
		// 向きの変更
		void ChangeDirection();	
		// 活動状態にあるか？
		bool IsActive();
		// ボス戦闘フラグをオンにするか
		bool IsBattle();
		// 移動処理
		void Move() override;
		// 浮遊処理
		bool Floating();
		// 連続攻撃
		void Rash();
		// 魂の生成
		void AddSoul();
		// 攻撃処理
		void Attack();
		// 重力処理を行うか？
		bool IsGravity();
		// 攻撃を終了するか？（マップチップと衝突しているか？）
		bool IsAttackEnd();
		// 怒り状態への遷移
		bool AngerOn();
		// 自機は左右どちらにいるのか？
		bool IsPlayerPos(double px);
		// 死んでいるか？
		bool IsDead() override;
		// 死亡処理の起動
		bool DeathOn();
		// 他ドール(敵)を抜け殻にする
		bool DollsEnd();
		// 現在の当たり判定の算出
		AABB NowCollision(std::string key) override;
		// 落下攻撃時の急所生成
		std::pair<AABB, AABB> BlinkVitalPart(Collision& col, int vital);
		// 自機アクションとの衝突判定
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
		// ワープエフェクトの生成(引数1:生成地点)
		bool AddWarpEffect(Vector2 spwan);
		// 連撃エフェクトの生成
		bool AddRushEffect();
		// 落下攻撃エフェクトの生成
		bool AddBlinkEffect();
		// 煙(衝撃波)エフェクトの生成
		bool AddSmokeEffect();
		// 怒りエフェクトの生成
		bool AddAngerEffect();
		// デバフエフェクトの生成
		bool AddDebuffEffect();
		// 自機が左右どちらにいるかの判定
		bool IsPlayerPosition();
		// 攻撃判定の切り替え
		bool AttackBox(bool flag);
	};
}

