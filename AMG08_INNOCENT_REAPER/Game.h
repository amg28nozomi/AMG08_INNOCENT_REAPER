#pragma once
// #include <memory>
#include <tuple>
#include <memory>
#include "MapChips.h"

// #include <vector>

namespace inr {
	// ゲーム終了フラグ用
	constexpr auto GAME_PLAYING = false;
	constexpr auto GAME_END = true;

	constexpr auto LEVER_LR = 0;
	constexpr auto LEVER_UD = 1;
	constexpr auto KEY_JOYPAD = 2;

	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;

	constexpr auto WINDOW_W = 1920;
	constexpr auto WINDOW_H = 1080;
	constexpr auto WINDOW_B = 32;

	constexpr auto HALF_WINDOW_W = 1920 / 2;
	constexpr auto HALF_WINDOW_H = 1080 / 2;
#ifdef _DEBUG
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	constexpr auto PLAYER_WIDTH = 50;
	constexpr auto PLAYER_HEIGHT = 110;

	namespace key {
		constexpr auto SOUND_NUM = "num";

		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";
		constexpr auto SOUND_PLAYER_ROB = "player_rob";
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";
		
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";

		constexpr auto SOUND_PLAYER_DASH = "player_dash";
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";	// 魂の切り替え
		constexpr auto SOUND_PLAYER_HIT = "player_hit";
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";	// 死亡音
	}

	namespace system {
		constexpr auto SOUDN_GAME_START = "game_start";
		constexpr auto SOUND_GAME_END = "game_end";
		constexpr auto SOUDN_CANCEL = "cancel"; // cancel
		constexpr auto SOUND_PAUSE = "pause";
		constexpr auto SOUND_SELECT = "select";	// セレクト音

		constexpr auto MOVE_STAGE1 = "move_stage1";
		constexpr auto MOVE_STAGE2 = "move_stage2";
	}

	namespace bgm {
		constexpr auto SOUND_TITLE = "sound_title";
		constexpr auto SOUND_STAGE_0 = "sound_stage0";
		constexpr auto SOUND_STAGE_1 = "sound_stage1";
		constexpr auto SOUND_STAGE_2 = "sound_stage2";
		constexpr auto SOUND_STAGE_3 = "sound_stageb";
	}


	class Game {
	public:
		Game();
		~Game();
		void Init();	// 初期化
		void Input();	//　入力
		void Process();		// 更新
		void Draw();	// 描画

		// アナログスティックの横入力を取得
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		// アナログスティックの縦入力を取得
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		// アナログスティックの入力情報を取得（押下情報）
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		// キーボード入力の取得（押下情報）
		inline int GetInputKey() { return _inputKey; }
		// 
		inline int GetInputTrg() { return _inputTrg; }



		// 全情報取得
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// トリガ情報を取得
		inline int GetTrgKey() { return _trgKey; }
		// フレームカウンタを取得
		inline int GetFrameCount() { return _frameCount; }
		// プログラムを終了するか否か
		inline bool GetEndFlag() { return _endFlag; }
		// プログラム終了をする
		inline void ProgramEnd() { ++_endCount; }
		// Gameクラスの参照
		inline Game& GetGame() { return *this; }

#ifdef _DEBUG
		inline bool IsDebugMode() { return _debug; }	// 当たり判定を描画するか否か？
#endif

		// 各種参照ゲッター
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }

		// Vector2 RepairPosition(const Vector2 pos, const int width, const int height);

	private:
		// ObjectServer& _objServer;
		std::unique_ptr<ModeServer> _modeServer;	// モードサーバー
		std::unique_ptr<ObjectServer> _objServer;	// オブジェサーバー
		std::unique_ptr<GimmickServer> _gServer;
		std::unique_ptr<MapChips> _mapChips;	// マップチップ
		std::unique_ptr<Scenario> _scenario;	// シナリオ

		Vector2 _worldPosition;	// world座標


		std::tuple<int, int, int> _joyKey;	// ジョイパッドの入力情報
		int _inputKey;	// キー入力
		int _inputTrg;	// トリガ入力

		static int _trgKey;	// トリガー
		static int _frameCount;	// フレーム情報

		bool _endFlag;	// プログラムを終了するか否か
		int _endCount;	// ゲームが終了するまでのカウンタ

		void CountUp();
		
#ifdef _DEBUG
		bool _debug;	// デバッグ状態か否か
#endif
	};
}


