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
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ModeMain(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~ModeMain();
		/**
		 * @brief	初期化処理
		 */
		void Init() override;
		/**
		 * @brief	更新処理
		 */
		void Process() override;
		/**
		 * @brief 描画処理
		 */
		void Draw() override;
		/**
		 * @brief						ステージキーの切り替え
		 * @param nextStage	次のステージキー
		 */
		void ChangeKey(const std::string nextStage) override;
		// 切り替え
		bool IsKeyNull();

		bool GameOver();	// フラグ変更
		bool OpenBossStage();	// ボスステージへの扉を解放する
		inline std::string StageKey() { return _stageKey; }
		inline bool BossOpen() { return _bossOpen; }
		inline bool BossFlag() { return _bossBattle; }
		bool StageReset(); // ステージ情報を戻す（元ゲームオーバー）
		bool BossBattle();	// ボス戦開始フラグ
		bool BossEnd();	// ボス戦終了フラグ
		std::shared_ptr<EffectServer> GetEffectServer() { return _eServer; }
		std::unique_ptr<ImageServer>& GetItemImages() { return _messageServer; }
		std::unique_ptr<ItemServer>& GetItemServer() { return _itemServer; }
		std::unique_ptr<TutorialServer>& GetTutorialServer() { return _tutorialServer; }
	private:
		std::string _stageKey;	// ステージ検索用
		std::string _changeKey;		// ステージ切り替えよう
		std::unique_ptr<BackGround> _bg;	// 背景
		std::unique_ptr<StageUi> _stageUi;
		// std::unique_ptr<UI> _uiSoul;
		std::unique_ptr<Pause> _pause;
		std::shared_ptr<EffectServer> _eServer;
		std::unique_ptr<ImageServer> _messageServer;
		std::unique_ptr<ForeGround> _fg;	// 前景
		std::unique_ptr<ItemServer> _itemServer;
		std::unique_ptr<TutorialServer> _tutorialServer;

		bool _isReset;	// 現在のステージの初期化を行うか？（）
		bool _bossOpen;	// ボスステージへの扉は開かれているか？
		bool _bossBattle;	// ボス戦は行われているか？
		bool _isEnding;	// エンディングに移行するか
		int _endCount;	// 終了するまでの待ち時間

		Vector2 _worldPosition;	// ワールド座標

		bool IsStageChange();	// ステージの切り替えを行うか？
		void SetObjects();	// オブジェクトの生成（再配置）
		bool StageChange();		// ステージの切り替え

		bool BgmManage(std::string nextStage);
		std::string BgmKey(std::string key);	// キーの切り替え
		bool IsEnding();
	};

}
