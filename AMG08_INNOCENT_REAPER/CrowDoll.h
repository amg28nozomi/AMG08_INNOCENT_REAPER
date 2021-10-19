/*****************************************************************//**
 * @file   CrowDoll.h
 * @brief  ボスエネミーのクロウドールクラス（エネミーベースクラスのサブクラス）
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
		/**
		 * @brief						このエネミーはボスかどうか
		 * @return					trueを返す
		 */
		inline bool IsBoss() override { return true; }
	private:
		/**
		 * @brief クロウドールの状態を表す列挙型
		 *				死亡、待機、デバフ、咆哮(行動不能)、連続斬り、頭上からの強襲、伸びる爪、怯み、非活動状態
		 */
		enum class CrowState {
			DEATH, IDOL, DEBUFF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _crowState;		//!< クロウドールの状態
		Vector2 _target;				//!< 攻撃目標座標
		Vector2 _actionEnd;			//!< アクション終了
		Vector2 _warpPos;				//!< 転移後の座標
		int _life;							//!< 体力
		int _atkInterval;				//!< 攻撃の猶予時間
		int _debuffCount;				//!< デバフを発動するか？
		int _invincible;				//!< 無敵時間
		int _actionCount;				//!< ラッシュカウント
		int _debuffChage;				//!< デバフ溜め
		bool _setup;						//!< 準備は完了したか？
		bool _arm;							//!< 腕は生成したか？
		bool _debuff;						//!< デバフフラグ
		bool _isAnimation;			//!< モーション再生を行うか
		bool _isWarp;						//!< ワープを行うか？
		bool _isAnger;					//!< 怒りフラグ
		bool _wait;							//!< 待機フラグ
		/**
		 * @brief						状態遷移
		 * @param nextState	遷移する状態
		 * @param key				遷移する画像キー
		 */
		void ModeChange(CrowState nextState, std::string key);
		/**
		 * @brief		攻撃目標座標の更新
		 */
		void GetTarget();
		/**
		 * @brief		各種状態の管理
		 */
		void SetState();
		/**
		 * @brief		ワープ処理の起動
		 */
		void WarpOn();
		/**
		 * @brief		ワープ処理
		 */
		void Warp();
		/**
		 * @brief		活動開始処理
		 */
		void WakeUp();
		/**
		 * @brief		向きフラグの切り替え
		 */
		void ChangeDirection();	
		/**
		 * @brief		ボス戦闘フラグをオンにするかの判定
		 */
		void IsBattle();
		/**
		 * @brief		移動処理
		 */
		void Move() override;
		/**
		 * @brief		浮遊処理
		 */
		void Floating();
		/**
		 * @brief		連続攻撃処理
		 */
		void Rash();
		/**
		 * @brief		魂の生成処理
		 */
		void AddSoul();
		/**
		 * @brief		攻撃処理
		 */
		void Attack();
		/**
		 * @brief		重力処理を行うかの判定
		 */
		void IsGravity();
		/**
		 * @brief		周囲のドールを抜け殻にする
		 */
		void DollsEnd();
		/**
		 * @brief							自機アクションボックスとの衝突判定
		 * @param	ckey				対象の現在の状態(キー)
		 * @param	acollision	対象の当たり判定ボックス
		 * @param	direction		対象の向きフラグ
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
		/**
		 * @brief				ワープエフェクトの生成
		 * @param spwan	生成地点
		 */
		void AddWarpEffect(Vector2 spwan);
		/**
		 * @brief			連撃エフェクトの生成
		 */
		void AddRushEffect();
		/**
		 * @brief			落下攻撃エフェクトの生成
		 */
		void AddBlinkEffect();
		/**
		 * @brief			煙(衝撃波)エフェクトの生成
		 */
		void AddSmokeEffect();
		/**
		 * @brief			怒りエフェクトの生成
		 */
		void AddAngerEffect();
		/**
		 * @brief			デバフエフェクトの生成
		 */
		void AddDebuffEffect();
		/**
		 * @brief			怒り状態に突入しているかの判定
		 * @return		怒り状態の場合は1を返す
		 *						通常状態の場合は0を返す
		 */
		int IsAnger();
		/**
		 * @brief			活動状態かの判定
		 * @return		活動状態の場合はtrueを返す
		 *						非活動状態の場合はfalseを返す
		 */
		bool IsActive();
		/**
		 * @brief			魂を奪える状態かの判定
		 * @return		奪える場合はtrueを返す
		 *						奪えない場合はfalseを返す
		 */
		bool IsVital();
		/**
		 * @brief			攻撃を中断するかの判定
		 *						マップチップと衝突した場合は中断
		 * @return		衝突している場合はtrueを返す
		 *						衝突していない場合はfalseを返す
		 */
		bool IsAttackEnd();
		/**
		 * @brief			怒り状態への遷移を行うかの判定
		 * @return		残り体力が半分を切っている場合はteuwを返す
		 *						既に怒り状態もしくは条件を満たしていない場合はfalseを返す
		 */
		bool AngerOn();
		/**
		 * @brief			自機が左右どちら側にいるかの判定
		 * @param px	自機のX座標
		 * @return		左側に居る場合はtrue
		 *						右側に居る場合はfalseを返す
		 */
		bool IsPlayerPos(double px);
		/**
		 * @brief			死亡状態かの判定
		 * @return		死亡状態の場合はtrueを返す
		 *						それ以外の場合はfalseを返す
		 */
		bool IsDead() override;
		/**
		 * @brief				死亡処理の起動
		 * @return			起動に成功した場合はtrueを返す
		 *							失敗または条件を満たしていない場合はfalseを返す
		 */
		bool DeathOn();
		/**
		 * @brief				自機が左右どちらにいるかの判定
		 * @return 　　　右側に居る場合はtrueを返す
		 *							左側に居る場合はfalseを返す
		 */
		bool IsPlayerPosition();
		/**
		 * @brief				攻撃判定の切り替え
		 * @param flag	切り替え用フラグ
		 * @return			切り替えに成功した場合はtrueを返す
		 *							切り替えに失敗した場合はfalseを返す
		 */
		bool AttackBox(bool flag);
		/**
		 * @brief				現在の当たり判定の取得
		 * @param key		現在の状態キー
		 * @return			死亡状態の場合は抜け殻の当たり判定を返す
		 *							それ以外の場合は通常の当たり判定を返す
		 */
		AABB NowCollision(std::string key) override;
		/**
		 * @brief				落下攻撃時の急所の取得
		 * @param col		修正する当たり判定ボックス
		 * @param vital	急所範囲
		 * @return			左右の当たり判定をpair型で返す
		 */
		std::pair<AABB, AABB> BlinkVitalPart(Collision& col, int vital);
	};
}

