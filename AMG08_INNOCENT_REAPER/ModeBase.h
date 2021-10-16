/*****************************************************************//**
 * @file   ModeBase.h
 * @brief  モードのスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>

namespace inr {
	/** モードのコンパイル時定数 */
	namespace mode {
		// 各種モードのキー情報
		constexpr auto TITLE = "mode_title";	// タイトル
		constexpr auto MAIN = "mode_main";		// 本編
		constexpr auto FIN = "mode_end";		// 終了
	}
	/** 二重インクルード防止 */
	class Game;
	class ObjectServer;
	class ModeServer;
	/** モードベース */
	class ModeBase {
	protected:
		Game& _game;						//!< ゲームクラス参照
		static int _modeFrame;	//!< 経過フレーム
		bool _update;						//!< 処理を更新するかどうか
		bool _resetFlg;					//!< モード切り替え時に初期化するかどうか
		std::string _bgmKey;		//!< bgmのキー
		/**
		 * @brief		BGMの管理
		 * @return	BGMを停止した場合はtrueを返す
		 *					BGMを再生した場合はfalseを返す
		 */
		bool BgmManage();
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ModeBase(Game& game);
		/**
		 * @brief		デストラクタ
		 */
		~ModeBase();
		/**
		 * @brief		初期化処理
		 */
		virtual void Init();
		/**
		 * @brief		更新処理
		 */
		virtual void Process();	
		/**
		 * @brief		描画処理
		 */
		virtual void Draw();
		/**
		 * @brief		ステージキーの切り替え
		 * @param		次のステージキー
		 */
		virtual void ChangeKey(const std::string nextStage);
		/**
		 * @brief		初期化フラグのオン
		 */
		inline void Reset() { _resetFlg = true; }
		/**
		 * @brief		経過フレームの初期化
		 */
		inline void TimeClear() { _modeFrame = 0; };
		/**
		 * @brief		経過フレームを取得
		 * @return	経過フレームを返す
		 */
		inline int GetFrame() { return _modeFrame; }
		/**
		 * @brief		キーの取得
		 * @return	""を返す
		 */
		virtual inline std::string GetKey() { return ""; }
	};
}