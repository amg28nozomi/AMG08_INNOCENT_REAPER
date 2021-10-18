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
	constexpr auto GAME_PLAYING = false;			//!< ゲーム実行中
	constexpr auto GAME_END = true;						//!< ゲーム終了
	// ジョイパッドの入力情報取り出し用番号
	constexpr auto LEVER_LR = 0;							//!< ジョイパッドの縦入力
	constexpr auto LEVER_UD = 1;							//!< ジョイパッドの横入力
	constexpr auto KEY_JOYPAD = 2;						//!< ジョイパッドのボタン入力
	// 画面情報
	constexpr auto WINDOW_W = 1920;						//!< 画面の横サイズ
	constexpr auto WINDOW_H = 1080;						//!< 画面の縦サイズ
	constexpr auto WINDOW_B = 32;							//!< ビット数
	// ハーフサイズ
	constexpr auto HALF_WINDOW_W = 1920 / 2;	//!< 画面半分の横サイズ
	constexpr auto HALF_WINDOW_H = 1080 / 2;	//!< 画面半分の縦サイズ
	// 自機のサイズ情報
	constexpr auto PLAYER_WIDTH = 50;					//!< 自機の横サイズ
	constexpr auto PLAYER_HEIGHT = 110;				//!< 自機の縦サイズ
	/**   SEのコンパイル時定数 */
	namespace key {
		// 音源用キー(SoundServer)
		constexpr auto SOUND_NUM = "num";															//!< 音源無し
		// 自機SE
		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";							//!< 自機移動音SE1
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";							//!< 自機移動音SE2
		constexpr auto SOUND_PLAYER_ROB = "player_rob";								//!< 自機奪うSE
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";		//!< 自機与える(成功)SE
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";	//!< 自機与える(失敗)SE
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";							//!< 自機ジャンプSE
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";					//!< 自機着地1SE
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";					//!< 自機着地2SE
		constexpr auto SOUND_PLAYER_DASH = "player_dash";							//!< 自機ダッシュSE
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";					//!< 自機魂の切り替えSE
		constexpr auto SOUND_PLAYER_HIT = "player_hit";								//!< 自機ダメージSE
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";							//!< 自機死亡SE
	}
	/** システムのコンパイル時定数 */
	namespace system {
		constexpr auto SOUDN_GAME_START = "game_start";								//!< ゲーム開始SE
		constexpr auto SOUND_GAME_END = "game_end";										//!< ゲーム終了SE
		constexpr auto SOUDN_CANCEL = "cancel";												//!< キャンセルSE
		constexpr auto SOUND_PAUSE = "pause";													//!< ポーズSE
		constexpr auto SOUND_SELECT = "select";												//!< セレクト音SE
		constexpr auto MOVE_STAGE1 = "move_stage1";										//!< ステージ遷移1SE
		constexpr auto MOVE_STAGE2 = "move_stage2";										//!< ステージ遷移2SE
	}
	/** BGMのコンパイル時定数 */
	namespace bgm {
		// BGM
		constexpr auto SOUND_TITLE = "sound_title";										//!< タイトル(BGM)
		constexpr auto SOUND_STAGE_0 = "sound_stage0";								//!< 封印の祭壇(BGM)
		constexpr auto SOUND_STAGE_1 = "sound_stage1";								//!< 荒廃した町(BGM)
		constexpr auto SOUND_STAGE_2 = "sound_stage2";								//!< 契りの森(BGM)
		constexpr auto SOUND_STAGE_3 = "sound_stageb";								//!< ボス戦(BGM)
		constexpr auto SOUND_ENDING = "sound_ending";									//!< エンディング(BGM)
	}
	/** 二重インクルード防止 */
	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;
	/** ゲームクラス */
	class Game {
	public:
		/**
		 * @brief		コンストラクタ
	   */
		Game();
		/**
		 * @brief		デストラクタ
		 */
		~Game();
		/**
		 * @brief		初期化処理
		 */
		void Init();
		/**
		 * @brief		入力処理
		 */
		void Input();
		/**
		 * @brief		更新処理 
		 */
		void Process();
		/**
		 * @brief		描画処理
		 */
		void Draw();
		/**
		 * @brief		アナログスティックの横入力情報を取得
		 * @return	アナログスティックの横入力情報を返す
		 */
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		/**
		 * @brief		アナログスティックの縦入力情報を取得
		 * @return	アナログスティックの縦入力情報を返す
		 */
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		/**
		 * @brief		アナログスティックのボタン入力情報を取得（押下情報）
		 * @return	アナログスティックのボタン入力情報を返す（押下情報）
		 */
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		/**
		 * @brief		ジョイパッドの全入力情報を取得（押下情報）
		 * @return	ジョイパッドの全入力情報を返す（押下情報）
		 */
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		/**
		 * @brief		トリガ情報を取得
		 * @return	トリガ情報を返す
		 */
		inline int GetTrgKey() { return _trgKey; }
		/**
		 * @brief		フレームカウンタを取得
		 * @return	フレームカウンタを返す
		 */
		inline int GetFrameCount() { return _frameCount; }
		/**
		 * @brief		プログラム終了フラグの取得
		 * @return	プログラム終了フラグを返す
		 */
		inline bool GetEndFlag() { return _endFlag; }
		/**
		 * @brief		プログラム終了予約
		 */
		inline void ProgramEnd() { ++_endCount; }
		/**
		 * @brief		ゲームクラスの参照を取得
		 * @return	参照を返す
		 */
		inline Game& GetGame() { return *this; }
#ifdef _DEBUG
		/**
		 * @brief		デバッグフラグの取得(Debugビルド限定)
		 * @return	デバッグフラグを返す
		 */
		inline bool IsDebugMode() { return _debug; }
#endif
		/**
		 * @brief		モードサーバの取得
		 * @return	モードサーバの参照を返す
		 */
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		/**
		 * @brief		オブジェクトサーバの取得
		 * @return	オブジェクトサーバの参照を返す
		 */
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		/**
		 * @brief		ギミックサーバの取得
		 * @return	ギミックサーバの参照を返す
		 */
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		/**
		 * @brief		マップチップの取得
		 * @return	マップチップの参照を返す
		 */
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		/**
		 * @brief		シナリオの取得
		 * @return	シナリオの参照を返す
		 */
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }
	private:
		std::unique_ptr<ModeServer> _modeServer;		//!< モードサーバ
		std::unique_ptr<ObjectServer> _objServer;		//!< オブジェサーバ
		std::unique_ptr<GimmickServer> _gServer;		//!< ギミックサーバ
		std::unique_ptr<MapChips> _mapChips;				//!< マップチップ
		std::unique_ptr<Scenario> _scenario;				//!< シナリオ
		Vector2 _worldPosition;											//!< ワールド座標
		std::tuple<int, int, int> _joyKey;					//!< ジョイパッドの入力情報
		int _endCount;															//!< ゲームが終了するまでのカウンタ
		bool _endFlag;															//!< プログラムを終了するか否か
#ifdef _DEBUG
		bool _debug;																//!< デバッグ状態かのフラグ
#endif
		static int _trgKey;													//!< トリガ情報
		static int _frameCount;											//!< フレームカウンタ
		/**
		 * @brief	フレームカウンタの加算処理
		 */
		void CountUp();
	};
}


