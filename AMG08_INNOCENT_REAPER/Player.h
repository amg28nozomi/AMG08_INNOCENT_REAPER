#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
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


	class Player : public ObjectBase {
	public:
		Player(Game& game);
		~Player() override;

		void Init() override;
		void Process() override;
		void Draw() override;

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

		double _lastX;	// ダッシュの最大移動距離(座標)
		
		int _aFrame;	// アクション実行のためのフレーム
		int _aCount;	// アニメーションの添え字
		int _sounds;

		int _dashInterval;	// ダッシュの再使用間隔
		double _jumpPower;	// ジャンプ溜め
		bool _changeGraph;	// 描画グラフィック切り替え用フラグ
		bool _input;	//ダッシュしているかどうか（trueの場合は他アクションを実行できない）
		// std::string _divKey;	// 読み込み用識別キー
		Keys _divKey;	// 左辺:グラフィック用、右辺:サウンド用
		KeyDates _aMotions;	// first:キー, s.first:総フレーム数, SEの再生時間
		boxs _collisions;	// キー、軸平行境界線BOX(AABBクラス)

		// std::pair<double, double> _moveVector;	// 移動距離(左辺:x, 右辺:y)

		void AnimationInit();
		bool GraphResearch(int* gh);	// 識別
		int GetSize(const std::string& key);
		int SoundResearch(const std::string& key);
		int GetSoundFrame(const std::string& key);

		void PositionUpdate();
		// Action処理まとめ
		bool Action(int key);

		// アクション入力処理
		void InputDash(double x);
		
		// 各種アクション
		void Move(int lever); // 移動
		void Dash(); // ダッシュ
		void Jump(); // ジャンプ
		void Rob(double x, double y); // 奪う
		void Give(double x, double y); // 与える
		void ChangeSoul(); // 魂の切り替え
		void Damage(); // ダメージ(ノックバック)
		bool Dead(); // 死亡判定

		AABB GetAABB();

#ifdef _DEBUG
		void DebugInfo();
#endif
	};
}

// 当たり判定実行用の処理を別途作成（関数化）
