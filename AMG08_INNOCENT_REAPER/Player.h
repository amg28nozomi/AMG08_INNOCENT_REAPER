#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <queue>
#include <unordered_map>

namespace inr {
	// 検索用キー
	constexpr auto PKEY_DEATH = "player_death";	// 死亡（モーションがないのでダメージモーション流用）
	constexpr auto PKEY_IDOL = "player_idol";	// 待機
	constexpr auto PKEY_RUN = "player_run";		// 移動
	constexpr auto PKEY_DASH = "player_dash";	// ダッシュ
	constexpr auto PKEY_ROB = "player_rob";		// 魂を奪う
	constexpr auto PKEY_JUMP = "player_jump";	//　ジャンプ
	constexpr auto PKEY_GIVE = "player_give";	// 魂を与える
	constexpr auto PKEY_FALL = "player_fall";	// 落下
	constexpr auto PKEY_HIT = "player_hit";
	constexpr auto PKEY_CLIMB = "player_climb";	// 蔦登り

	constexpr auto PIMAGE_SIZE = 140;	// プレイヤーの描画サイズ
	constexpr auto PLAYER_DEATH_IMAGE = 400;

	// プレイヤーの向き
	constexpr auto PL_LEFT = true;
	constexpr auto PL_RIGHT = false;

	constexpr auto SOUL_MAX = 3;	// 保有できる魂の上限

	class SoulSkin;


	class Player : public ObjectBase {
	public:
		Player(Game& game);
		~Player() override;

		void Init() override;
		void Process() override;
		void Draw() override;

		inline Vector2 GetMoveVector() override{ return _moveVector; }
		inline bool HaveSoul() { return !_souls.empty(); }	// 与える魂を有しているか？
		inline bool IsSoulMax() { return _souls.size() == SOUL_MAX; };	// 魂は入手上限に到達しているか？
		inline int SoulsSize() { return static_cast<int>(_souls.size()); }
		inline void InputChange(bool input) { _input = input; }
		inline void InputOff() { _input = false; }

		bool Debuf();	// 被デバフ攻撃
		bool Damage(bool mv);
		bool KnockBack(bool mv);	// ノックバック処理
		bool Reset();	// 開始地点に戻す

		void SetParameter(ObjectValue objValue) override;	// 引数1:初期座標　引数2:魂を保有しているか　引数3:
		void SetParameter(std::pair<Vector2, bool> newdata, std::string sKey);
		void SoulCatch(std::shared_ptr<SoulSkin> soul);
		std::shared_ptr<SoulSkin> GiveSoul();	// 魂の譲渡
		inline std::queue<std::shared_ptr<SoulSkin>> GetSouls() { return _souls; }

	private:
		// アクションの状態
		enum class ActionState {
			// 死亡、待機、移動、ダッシュ、奪う、与える、ノックバック、ツタ登り
			DEATH, IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT, GRAB
		};
		enum class State {
			// 生存、ダメージ、死亡
			ALLIBE, DAMEGE, DEAD
		};

		ActionState _aState;
		Vector2 _moveVector;	// 移動量(単位ベクトル)
		std::queue<std::shared_ptr<SoulSkin>> _souls;

		double _dashX;	// ダッシュの最大移動距離(座標)
		double _moveD;	// 移動倍率

		int _pal;
		
		int _judegFrame;	// 判定フレーム数
		int _aFrame;	// アクション実行のためのフレーム
		int _sounds;
		int _debuffCount;	// デバフカウンタ

		int _hitCount;	// 判定フレーム(アクション)

		int _dashInterval;	// ダッシュの再使用間隔
		int _knockBack;	// ノックバックカウンタ
		int _invincible;	// 無敵時間
		std::string _moveType;	// 移動音
		std::string _landingType;	// 着地音
		double _jumpPower;	// ジャンプ溜め
		std::pair<double, double> _ivx;
		bool _input;	// 入力処理を受け付けるか
		bool _grab;	// 掴み判定
		bool _sChange;	// ステージが変わったかどうか
		bool _ivxInterval;	// 棘ダメージ後のフラグ
		// std::string _divKey;	// 読み込み用識別キー
		boxs _collisions;	// キー、軸平行境界線BOX(AABBクラス)

		// std::pair<double, double> _moveVector;	// 移動距離(左辺:x, 右辺:y)

		void AnimationInit();
		void ChangeState(Player::ActionState nextState, std::string key);	// 状態遷移

		void PositionUpdate();
		// Action処理まとめ
		bool Action(int key);

		// アクション入力処理
		void InputDash(double x);
		void InputJump();
		
		// 各種アクション
		void Move(int lever); // 移動
		void Move();	// キー入力対応
		void Climb(int leverUD);	// 上下移動
		void Dash(); // ダッシュ
		void Jump(); // ジャンプ
		void Rob(double x, double y); // 奪う
		void Give(double x, double y); // 与える
		void ChangeSoul(); // 魂の切り替え
		// bool IsDamage(); // ダメージ判定を受けるか？
		bool Dead(); // 死亡判定
		void Death();	// 死亡処理（）
		bool IsStandChip() override;

		// 棘のダメージ処理
		void DamageThorm();

		void IsGrab();	// 当たり判定等の処理
		void Grab();

		void StateUpdate();	// 状態遷移およびその他処理

		AABB GetAABB();
		AABB NowCollision(std::string key) override;

		double GetFix(double value);	// 向きに応じて反転処理をかける
		
		void InputOn();	// 入力を再開するか
		
#ifdef _DEBUG
		void DebugInfo();
#endif
	};
}

// 当たり判定実行用の処理を別途作成（関数化）
