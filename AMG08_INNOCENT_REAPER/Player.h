/*****************************************************************//**
 * @file   Player.h
 * @brief  自機クラス（オブジェクトベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <queue>
#include <unordered_map>

namespace inr {
	// 検索用キー
	constexpr auto PKEY_DEATH = "player_death";	//!< 死亡（モーションがないのでダメージモーション流用）
	constexpr auto PKEY_IDOL = "player_idol";		//!< 待機
	constexpr auto PKEY_RUN = "player_run";			//!< 移動
	constexpr auto PKEY_DASH = "player_dash";		//!< ダッシュ
	constexpr auto PKEY_ROB = "player_rob";			//!< 魂を奪う
	constexpr auto PKEY_JUMP = "player_jump";		//!<　ジャンプ
	constexpr auto PKEY_GIVE = "player_give";		//!< 魂を与える
	constexpr auto PKEY_FALL = "player_fall";		//!< 落下
	constexpr auto PKEY_HIT = "player_hit";			//!< ダメージ
	constexpr auto PKEY_CLIMB = "player_climb";	//!< 蔦登り
	constexpr auto PIMAGE_SIZE = 140;						//!< プレイヤーの描画サイズ
	constexpr auto PLAYER_DEATH_IMAGE = 400;		//!< 死亡エフェクトのサイズ
	// プレイヤーの向き
	constexpr auto PL_LEFT = true;							//!< 左向き
	constexpr auto PL_RIGHT = false;						//!< 右向き
	constexpr auto SOUL_MAX = 3;								//!< 保有できる魂の上限
	/** 二重インクルード防止 */
	class SoulSkin;
	/** 自機 */
	class Player : public ObjectBase {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 */
		Player(Game& game);
		/**
		 * @brief						デストラクタ
		 */
		~Player() override;
		/**
		 * @brief						初期化処理
		 */
		void Init() override;
		/**
		 * @brief						更新処理
		 */
		void Process() override;
		/**
		 * @brief						描画処理
		 */
		void Draw() override;
		/**
		 * @brief						移動ベクトルの取得
		 * @return					移動ベクトルを返す
		 */
		inline Vector2 GetMoveVector() override{ return _moveVector; }
		/**
		 * @brief						魂を保有しているかの取得
		 * @return					要素がある場合はtrueを返す
		 *									空の場合はfalseを返す
		 */
		inline bool HaveSoul() { return !_souls.empty(); }
		/**
		 * @brief						魂は入手上限に到達しているか
		 * @return					到達している場合はtrueを返す
		 *									到達していない場合はfalseを返す
		 */
		inline bool IsSoulMax() { return _souls.size() == SOUL_MAX; };
		/**
		 * @brief						魂所持数の取得
		 * @return					要素数を返す
		 */
		inline int SoulsSize() { return static_cast<int>(_souls.size()); }
		/**
		 * @brief						入力受付の停止
		 */
		inline void InputOff() { _input = false; }
		/**
		 * @brief						デバフ処理
		 * @return					デバフ処理に成功した場合はtrueを返す
		 */
		bool Debuff();
		/**
		 * @brief						ダメージ処理
		 * @param	mv				ノックバック方向
		 * @return					死亡状態に遷移した場合はtrueを返す
		 *									生存している場合はfalseを返す
		 */
		bool Damage(bool mv);
		/**
		 * @brief						ノックバック処理
		 * @param	mv				ノックバック方向
		 * @return					処理に成功した場合はtrueを返す
		 *									無敵時間中の場合はfalseを返す
		 */
		bool KnockBack(bool mv);
		/**
		 * @brief						リセット処理
		 * @return					処理に成功した場合はtrueを返す
		 */
		bool Reset();
		/**
		 * @brief						オブジェクト情報の登録
		 * @param objValue	オブジェクト情報
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief						座標の更新
		 * @param newdata		左辺:座標　右辺:向き
		 * @param sKey			ステージキー
		 */
		void SetParameter(std::pair<Vector2, bool> newdata, std::string sKey);
		/**
		 * @brief						魂の登録
		 * @param soul			魂
		 */
		void SoulCatch(std::shared_ptr<SoulSkin> soul);
		/**
		 * @brief						魂の譲渡
		 * @return					譲渡する魂を返す
		 */
		std::shared_ptr<SoulSkin> GiveSoul();
		/**
		 * @brief						魂の取得
		 * @return					魂を格納するコンテナを返す
		 */
		inline std::queue<std::shared_ptr<SoulSkin>> GetSouls() { return _souls; }
	private:
		/**
		 * @brief						アクション状態を表す列挙型
		 *									死亡、待機、移動、ダッシュ、奪う、与える、ノックバック、ツタ登り
		 */
		enum class ActionState {
			DEATH, IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT, GRAB
		};
		/**
		 * @brief						状態を表す列挙型
		 *									生存、ダメージ、死亡
		 */
		enum class State {
			ALLIBE, DAMEGE, DEAD
		};
		ActionState _aState;														//!< アクション状態
		Vector2 _moveVector;														//!< 移動量(単位ベクトル)
		std::queue<std::shared_ptr<SoulSkin>> _souls;		//!< 魂を格納するコンテナ
		int _judegFrame;																//!< 判定フレーム数
		int _aFrame;																		//!< アクション実行のためのフレーム
		int _debuffCount;																//!< デバフカウンタ
		int _hitCount;																	//!< 判定フレーム(アクション)
		int _dashInterval;															//!< ダッシュの再使用間隔
		int _knockBack;																	//!< ノックバックカウンタ
		int _invincible;																//!< 無敵時間
		double _dashX;																	//!< ダッシュの最大移動距離(座標)
		double _moveD;																	//!< 移動倍率
		double _jumpPower;															//!< ジャンプ溜め
		bool _input;																		//!< 入力処理を受け付けるか
		bool _grab;																			//!< 掴み判定
		bool _sChange;																	//!< ステージが変わったかどうか
		bool _ivxInterval;															//!< 棘ダメージ後のフラグ
		std::string _moveType;													//!< 移動音
		std::string _landingType;												//!< 着地音
		std::pair<double, double> _ivx;									//!< 蔦チップの座標
		boxs _collisions;																//!< 当たり判定ボックス格納用コンテナ
		/**
		 * @brief						状態遷移
		 * @param nextState	遷移する状態
		 * @param key				遷移後のキー
		 */
		void ChangeState(Player::ActionState nextState, std::string key);
		/**
		 * @brief						座標情報等の更新
		 */
		void PositionUpdate();
		/**
		 * @brief						アクション処理
		 * @param key				ボタンの入力情報
		 */
		bool Action(int key);
		/**
		 * @brief						ダッシュの始動
		 */
		void InputDash();
		/**
		 * @brief						ジャンプの始動
		 */
		void InputJump();
		/**
		 * @brief						移動
		 * @param lever			アナログスティックの横入力情報
		 */
		void Move(int lever);
		/**
		 * @brief						掴み
		 * @param leverUD		アナログスティックの縦入力情報
		 */
		void Climb(int leverUD);
		/**
		 * @brief						ダッシュ
		 */
		void Dash();
		/**
		 * @brief						ジャンプ
		 */
		void Jump();
		/**
		 * @brief						奪うアクション
		 */
		void Rob();
		/**
		 * @brief						与えるアクション
		 */
		void Give();
		/**
		 * @brief						魂の切り替え処理
		 */
		void ChangeSoul();
		/**
		 * @brief						死亡判定
		 * @return					死亡している場合はtrueを返す
		 *									生存している場合はfalseを返す
		 */
		bool Dead();
		/**
		 * @brief						死亡処理
		 */
		void Death();
		/**
		 * @brief						チップに立っているかの判定
		 * @return					接触している場合はtrueを返す
		 *									接触していない場合はfalseを返す
		 */
		bool IsStandChip() override;
		/**
		 * @brief						棘のダメージ処理
		 */
		void DamageThorm();
		/**
		 * @brief						掴み状態かの判定
		 */
		void IsGrab();
		/**
		 * @brief						掴み処理
		 */
		void Grab();
		/**
		 * @brief						状態遷移およびその他処理
		 */
		void StateUpdate();
		/**
		 * @brief						現在の当たり判定の取得
		 * @param key				現在の状態キー
		 * @return					ダッシュ状態の場合はダッシュ時の当たり判定を返す
		 *									それ以外の場合は通常の当たり判定を返す
		 */
		AABB NowCollision(std::string key) override;
		/**
		 * @brief						入力受付の再開
		 */
		void InputOn();
#ifdef _DEBUG
		/**
		 * @brief						デバッグ情報の描画(Debug限定)
		 */
		void DebugInfo();
#endif
	};
}
