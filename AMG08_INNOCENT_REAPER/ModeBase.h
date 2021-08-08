#pragma once
#include <unordered_map>

namespace inr {

	namespace mode {
		constexpr auto TITLE = "mode_title";	// タイトル
		constexpr auto SELECT = "mode_select";	// セレクト画面
		constexpr auto MAIN = "mode_main";	// 本編
		constexpr auto GAMEOVER = "mode_gameover";	// ゲームオーバー
		constexpr auto FIN = "mode_end";	// 終了
	}


	class Game;
	class ObjectServer;
	class ModeServer;

	class ModeBase {
	protected:
		static int _modeFrame;	// 経過フレーム
		bool _update;	// 処理を更新するかどうか
		bool _resetFlg; // モード切り替え時に初期化するかどうか
		Game& _game;

		using ModeData = std::unordered_map<std::string, int>;
	public:
		ModeBase(Game& game);
		~ModeBase();
		virtual void Init();
		virtual void Process();	
		virtual void Draw();
		inline void Reset() { _resetFlg = true; } // 初期化フラグオン
		inline void TimeClear() { _modeFrame = 0; };	// フレーム初期化
		inline int GetFrame() { return _modeFrame; }	// 経過フレームを取得
	};
}

//// タイトル、チュートリアル、本編、エンディング、終了
		//enum class ModeState {
		//	TITLE, TUTORIAL, MAIN_GAME, ENDING, FIN
		//};
//ModeState _mode;	// 何のモードか

//inline ModeState ModeState() { return _mode; }	// Stateを取得

// 