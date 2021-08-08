#pragma once
// #include <memory>
#include <tuple>
#include <memory>
#include "MapChips.h"

// #include <vector>

namespace inr {

	constexpr auto LEVER_LR = 0;
	constexpr auto LEVER_UD = 1;
	constexpr auto KEY_JOYPAD = 2;

	class ObjectServer;
	class ModeServer;

	constexpr auto WINDOW_W = 1920;
	constexpr auto WINDOW_H = 1080;
	constexpr auto WINDOW_B = 32;
#ifdef _DEBUG
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	constexpr auto PLAYER_WIDTH = 90;
	constexpr auto PLAYER_HIGHT = 180;

	namespace key {
		constexpr auto SOUND_NUM = "num";

		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";
		constexpr auto SOUND_PLAYER_ROB = "player_rob";
		constexpr auto SOUND_PLAYER_GIVE = "player_give";
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";
		constexpr auto SOUND_PLAYER_FALL = "player_fall";
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
		// 全情報取得
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// トリガ情報を取得
		inline int GetTrgKey() { return _trgKey; }

		inline int GetFrameCount() { return _frameCount; }

		inline Game& GetGame() { return *this; }

		// 各種参照ゲッター
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }

		// Vector2 RepairPosition(const Vector2 pos, const int width, const int height);

	private:
		// ObjectServer& _objServer;
		std::unique_ptr<ModeServer> _modeServer;	// モードサーバー
		std::unique_ptr<ObjectServer> _objServer;	// オブジェサーバー
		std::unique_ptr<MapChips> _mapChips;	// マップチップ

		


		std::tuple<int, int, int> _joyKey;	// ジョイパッドの入力情報

		static int _trgKey;	// トリガー
		static int _frameCount;	// フレーム情報
		// int _leverLR;	// ジョイパッドのレバー情報(左右)
		// int _leverUD;	// ジョイパッドのレバー情報
		// int _inputKey;	// ジョイパッドの入力状態
	};
}


