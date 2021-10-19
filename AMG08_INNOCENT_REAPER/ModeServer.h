/*****************************************************************//**
 * @file   ModeServer.h
 * @brief  各種モードを管理するモードサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "FadeBlack.h"

namespace inr {
	/** 二重インクルード防止 */
	class Game;
	class ModeMain;
	/** モードサーバ */
	class ModeServer {
	public:
		/**
		 * @brief					 コンストラクタ
		 * @param game		 ゲームクラスの参照
		 */
		ModeServer(Game& game);
		/**
		 * @brief					 デストラクタ
		 */
		~ModeServer();
		/**
		 * @brief					 初期化処理
		 */
		void Init();
		/**
		 * @brief					 更新処理
		 */
		void Process();
		/**
		 * @brief					 描画処理
		 */
		void Draw();
		/**
		 * @brief					 モードの切り替え準備
		 * @param necxMode 次のモード
		 * @param	interval 遷移までに要するインターバル
		 */
		void ModeChange(std::string nextMode, int interval = 0);
		/**
		 * @brief					 ゲームの終了処理
		 */
		void GameEnd();
		/**
		 * @brief					 ゲーム本編の取得
		 * @return				 ゲーム本編のシェアードポインタを返す
		 */
		std::shared_ptr<ModeMain> GetModeMain();
		/**
		 * @brief					 フェードアウト処理呼び出し
		 */
		inline void FadeOut() { _fadeBlack->FlagChange(image::FADE_OUT, 60); }
		/**
		 * @brief					 加算・減算処理が終了しているかの判定
		 * @return				 戻り値を返す
		 */
		inline bool PalChange() { return _fadeBlack->PalChange(); }
		/**
		 * @brief					 終了フラグの取得
		 * @return				 終了フラグを返す
		 */
		inline bool IsFadeEnd() { return _fadeBlack->IsEnd(); }
	private:
		/** モードを格納する連想配列(値:キー　値:モード) */
		using ModeMap = std::unordered_map<std::string, std::shared_ptr<ModeBase>>;
		Game& _game;														//!< ゲームクラスの参照
		ModeMap _modes;													//!< モード用コンテナ
		std::unique_ptr<FadeBlack> _fadeBlack;	//!< フェード
		std::string _modeKey;										//!< モード検索用キー
		std::string _ChangeKey;									//!< モード切り替え用キー
		/**
		 * @brief					 モードの開放
		 */
		void ClearModeLists();
		/**
		 * @brief					 モードの切り替え
		 */
		void IsModeChange();
		/**
		 * @brief					 モードの初期化
		 * @return				 初期化に成功した場合はtrueを返す
		 *								 失敗した場合はfalseを返す
		 */
		bool ModeInit();
	};

}
