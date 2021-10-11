/*****************************************************************//**
 * \file   Game.h
 * \brief  ゲームクラス
 *		   本ゲームの処理を回しています
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <memory>
#include "MapChips.h"

namespace inr {
	// ゲーム終了フラグ用
	constexpr auto GAME_PLAYING = false;
	constexpr auto GAME_END = true;
	// ジョイパッドの入力情報取り出し用番号
	constexpr auto LEVER_LR = 0;		// 縦入力
	constexpr auto LEVER_UD = 1;		// 横入力
	constexpr auto KEY_JOYPAD = 2;		// ボタン入力
	// 二重インクルード防止
	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;
	// 画面情報
	constexpr auto WINDOW_W = 1920;				// 横サイズ
	constexpr auto WINDOW_H = 1080;				// 縦サイズ
	constexpr auto WINDOW_B = 32;				// ビット数
	// ハーフサイズ
	constexpr auto HALF_WINDOW_W = 1920 / 2;
	constexpr auto HALF_WINDOW_H = 1080 / 2;
#ifdef _DEBUG
	// デバッグ時の画像サイズ
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	// 自機のサイズ情報
	constexpr auto PLAYER_WIDTH = 50;
	constexpr auto PLAYER_HEIGHT = 110;

	namespace key {
		// 音源用キー(SoundServer)
		constexpr auto SOUND_NUM = "num";								// 音源無し
		// 自機SE
		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";				// 移動音1
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";				// 移動音2
		constexpr auto SOUND_PLAYER_ROB = "player_rob";					// 奪う
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";		// 与える(成功)
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";	// 与える(失敗)
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";				// ジャンプ
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";			// 着地1
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";			// 着地2
		constexpr auto SOUND_PLAYER_DASH = "player_dash";				// ダッシュ
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";			// 魂の切り替え
		constexpr auto SOUND_PLAYER_HIT = "player_hit";					// ダメージ
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";				// 死亡
	}
	namespace system {
		// システムSE
		constexpr auto SOUDN_GAME_START = "game_start";					// ゲーム開始
		constexpr auto SOUND_GAME_END = "game_end";						// ゲーム終了
		constexpr auto SOUDN_CANCEL = "cancel";							// キャンセル
		constexpr auto SOUND_PAUSE = "pause";							// ポーズ
		constexpr auto SOUND_SELECT = "select";							// セレクト音
		constexpr auto MOVE_STAGE1 = "move_stage1";						// ステージ遷移1
		constexpr auto MOVE_STAGE2 = "move_stage2";						// ステージ遷移2
	}
	namespace bgm {
		// BGM
		constexpr auto SOUND_TITLE = "sound_title";						// タイトル
		constexpr auto SOUND_STAGE_0 = "sound_stage0";					// 封印の祭壇
		constexpr auto SOUND_STAGE_1 = "sound_stage1";					// 荒廃した町
		constexpr auto SOUND_STAGE_2 = "sound_stage2";					// 契りの森
		constexpr auto SOUND_STAGE_3 = "sound_stageb";					// ボス戦
		constexpr auto SOUND_ENDING = "sound_ending";					// エンディング
	}
	// ゲームクラス
	class Game {
	public:
		// コンストラクタ
		Game();
		// デストラクタ
		~Game();
		// 初期化
		void Init();
		// 入力状態の更新
		void Input();
		// 更新
		void Process();
		// 描画
		void Draw();
		// アナログスティックの横入力を取得
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		// アナログスティックの縦入力を取得
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		// アナログスティックの入力情報を取得（押下情報）
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		// ジョイパッドの全入力情報を取得
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// トリガ情報を取得
		inline int GetTrgKey() { return _trgKey; }
		// フレームカウンタを取得
		inline int GetFrameCount() { return _frameCount; }
		// プログラムを終了をするかの取得
		inline bool GetEndFlag() { return _endFlag; }
		// プログラム終了をする
		inline void ProgramEnd() { ++_endCount; }
		// Gameクラスの参照
		inline Game& GetGame() { return *this; }
#ifdef _DEBUG
		// デバッグモードフラグの取得
		inline bool IsDebugMode() { return _debug; }
#endif
		// モードサーバの取得
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		// オブジェクトサーバの取得
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		// ギミックサーバの取得
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		// マップチップの取得
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		// シナリオの取得
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }
	private:
		std::unique_ptr<ModeServer> _modeServer;	// モードサーバ
		std::unique_ptr<ObjectServer> _objServer;	// オブジェサーバ
		std::unique_ptr<GimmickServer> _gServer;	// ギミックサーバ
		std::unique_ptr<MapChips> _mapChips;		// マップチップ
		std::unique_ptr<Scenario> _scenario;		// シナリオ
		Vector2 _worldPosition;						// ワールド座標
		std::tuple<int, int, int> _joyKey;			// ジョイパッドの入力情報
		int _endCount;								// ゲームが終了するまでのカウンタ
		bool _endFlag;								// プログラムを終了するか否か
#ifdef _DEBUG
		bool _debug;								// デバッグ状態か否か
#endif
		static int _trgKey;							// トリガー
		static int _frameCount;						// フレーム情報
		// フレームカウンタの加算処理
		void CountUp();
	};
}


