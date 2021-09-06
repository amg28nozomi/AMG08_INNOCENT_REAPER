#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <queue>
#include <unordered_map>

namespace inr {
	// 検索用キー
	constexpr auto PKEY_IDOL = "player_idol";	// 待機
	constexpr auto PKEY_RUN = "player_run";		// 移動
	constexpr auto PKEY_DASH = "player_dash";	// ダッシュ
	constexpr auto PKEY_ROB = "player_rob";		// 魂を奪う
	constexpr auto PKEY_JUMP = "player_jump";	//　ジャンプ
	constexpr auto PKEY_GIVE = "player_give";	// 魂を与える
	constexpr auto PKEY_FALL = "player_fall";	// 落下
	constexpr auto PKEY_HIT = "player_hit";

	constexpr auto PIMAGE_SIZE = 140;	// プレイヤーの描画サイズ

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

		bool Damage(bool mv);

		void SetParameter(ObjectValue objValue) override;	// 引数1:初期座標　引数2:魂を保有しているか　引数3:
		void SoulCatch(std::shared_ptr<SoulSkin> soul);
		std::shared_ptr<SoulSkin> GiveSoul();	// 魂の譲渡
		inline std::queue<std::shared_ptr<SoulSkin>> GetSouls() { return _souls; }

	private:
		// アクションの状態
		enum class ActionState {
			// 待機、移動、ダッシュ、奪う、与える、ノックバック
			IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT
		};
		enum class State {
			// 生存、ダメージ、死亡
			ALLIBE, DAMEGE, DEAD
		};

		ActionState _aState;
		Vector2 _moveVector;	// 移動量(単位ベクトル)
		std::queue<std::shared_ptr<SoulSkin>> _souls;

		double _dashX;	// ダッシュの最大移動距離(座標)
		
		int _judegFrame;	// 判定フレーム数
		int _aFrame;	// アクション実行のためのフレーム
		int _sounds;

		int _hitCount;	// 判定フレーム(アクション)

		int _dashInterval;	// ダッシュの再使用間隔
		int _knockBack;	// ノックバックカウンタ
		int _invincible;	// 無敵時間
		double _jumpPower;	// ジャンプ溜め
		bool _input;	// 入力処理を受け付けるか
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
		void Dash(); // ダッシュ
		void Jump(); // ジャンプ
		void Rob(double x, double y); // 奪う
		void Give(double x, double y); // 与える
		void ChangeSoul(); // 魂の切り替え
		// bool IsDamage(); // ダメージ判定を受けるか？
		bool Dead(); // 死亡判定
		bool IsStandChip() override;
		void DamageThorm();	// 棘のダメージ処理

		void StateUpdate();	// 状態遷移およびその他処理

		AABB GetAABB();
		AABB NowCollision(std::string key) override;

#ifdef _DEBUG
		void DebugInfo();
#endif
	};
}

// 当たり判定実行用の処理を別途作成（関数化）
