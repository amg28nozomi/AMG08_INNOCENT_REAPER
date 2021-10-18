/*****************************************************************//**
 * @file   ModeMain.h
 * @brief  ゲーム本編を管理するモードメインクラス（モードベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "Vector2.h"
#include "ImageServer.h"
#include <memory>

namespace inr {
	/** 二重インクルード防止 */
	class Pause;
	class EffectServer;
	class StageUi;
	class ForeGround;
	class ItemServer;
	class TutorialServer;
	/** ゲーム本編 */
	class ModeMain : public ModeBase {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 */
		ModeMain(Game& game);
		/**
		 * @brief						デストラクタ
		 */
		~ModeMain();
		/**
		 * @brief						初期化処理
		 */
		void Init() override;
		/**
		 * @brief						更新処理
		 */
		void Process() override;
		/**
		 * @brief						描画処理
		 */
		void Draw() override;
		/**
		 * @brief						ステージキーの切り替え
		 * @param nextStage	次のステージキー
		 */
		void ChangeKey(const std::string nextStage) override;
		/**
		 * @brief						切り替え用キーが空かの判定
		 * @return					切り替わっていない場合はtrueを返す
		 *									切り替わっている場合はfalseを返す
		 */
		bool IsKeyNull();
		/**
		 * @brief						ゲームオーバー
		 * @return					ゲームオーバー処理に成功した場合はtrueを返す
		 *									失敗した場合はfalseを返す
		 */
		bool GameOver();
		/**
		 * @brief						ボスステージへの扉を解放する
		 */
		bool OpenBossStage();
		/**
		 * @brief						ステージキーの取得
		 * @return					ステージキーを返す
		 */
		inline std::string StageKey() { return _stageKey; }
		/**
		 * @brief						ボス扉の開閉フラグの取得
		 * @return					ボス扉の階へフラグを返す
		 */
		inline bool BossOpen() { return _bossOpen; }
		/**
		 * @brief						ボス戦フラグの取得
		 * @return					ボス戦フラグを返す
		 */
		inline bool BossFlag() { return _bossBattle; }
		/**
		 * @brief						ステージ情報のリセット
		 * @return					リセットに成功した場合はtrueを返す
		 *									リセットに失敗した場合はfalseを返す
		 */
		bool StageReset();
		/**
		 * @brief						ボス戦開始
		 * @return					ボス戦を開始した場合はtrueを返す
		 *									失敗した場合はfalseを返す
		 */
		bool BossBattle();
		/**
		 * @brief						ボス戦を終了するか		
		 * @return					ボス戦を終了した場合はtrueを返す
		 *									失敗した場合はfalseを返す
		 */
		bool BossEnd();
		/**
		 * @brief						エフェクトサーバの取得
		 * @return					エフェクトサーバを返す
		 */
		std::shared_ptr<EffectServer> GetEffectServer() { return _eServer; }
		/**
		 * @brief						メッセージサーバの取得
		 * @return					メッセージサーバを返す
		 */
		std::unique_ptr<ImageServer>& GetItemImages() { return _messageServer; }
		/**
		 * @brief						アイテムサーバの取得
		 * @return					アイテムサーバを返す
		 */
		std::unique_ptr<ItemServer>& GetItemServer() { return _itemServer; }
		/**
		 * @brief						チュートリアルサーバの取得
		 * @return					チュートリアルサーバを返す
		 */
		std::unique_ptr<TutorialServer>& GetTutorialServer() { return _tutorialServer; }
	private:
		std::string _stageKey;														//!< ステージ検索用
		std::string _changeKey;														//!< ステージ切り替えよう
		std::unique_ptr<BackGround> _bg;									//!< 背景
		std::unique_ptr<StageUi> _stageUi;								//!< ステージUI
		// std::unique_ptr<UI> _uiSoul;
		std::unique_ptr<Pause> _pause;										//!< ポーズ
		std::shared_ptr<EffectServer> _eServer;						//!< エフェクトサーバ
		std::unique_ptr<ImageServer> _messageServer;			//!< メッセージサーバ
		std::unique_ptr<ForeGround> _fg;									//!< 前景
		std::unique_ptr<ItemServer> _itemServer;					//!< アイテムサーバ
		std::unique_ptr<TutorialServer> _tutorialServer;	//!<  チュートリアルサーバ

		bool _isReset;	// 現在のステージの初期化を行うか？（）
		bool _bossOpen;	// ボスステージへの扉は開かれているか？
		bool _bossBattle;	// ボス戦は行われているか？
		bool _isEnding;	// エンディングに移行するか
		int _endCount;	// 終了するまでの待ち時間

		Vector2 _worldPosition;	// ワールド座標

		bool IsStageChange();	// ステージの切り替えを行うか？
		bool StageChange();		// ステージの切り替え

		bool BgmManage(std::string nextStage);
		std::string BgmKey(std::string key);	// キーの切り替え
		bool IsEnding();
	};

}
