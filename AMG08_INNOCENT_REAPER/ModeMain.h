#pragma once
#include "ModeBase.h"
#include "BackGround.h"
// #include "UI.h"
#include "Vector2.h"
#include "ImageServer.h"
#include <memory>

namespace inr {

	class Pause;
	class EffectServer;
	class StageUi;
	class ForeGround;
	class ItemServer;
	class TutorialServer;

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;

		void ChangeKey(const std::string nextStage) override;
		bool IsKeyNull();	// キーは空か？

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
