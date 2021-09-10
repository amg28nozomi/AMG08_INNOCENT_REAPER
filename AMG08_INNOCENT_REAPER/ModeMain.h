#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "UI.h"
#include "Vector2.h"
#include <memory>

namespace inr {

	class Pause;

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;

		void ChangeKey(const std::string nextStage) override;
		bool IsKeyNull();	// キーは空か？

		inline std::string StageKey() { return _stageKey; }
	private:
		std::string _stageKey;	// ステージ検索用
		std::string _changeKey;		// ステージ切り替えよう
		std::unique_ptr<BackGround> _bg;	// 背景
		std::unique_ptr<UI> _uiSoul;
		std::unique_ptr<Pause> _pause;

		Vector2 _worldPosition;	// ワールド座標

		bool IsStageChange();	// ステージの切り替えを行うか？
		void SetObjects();	// オブジェクトの生成（再配置）
		bool StageChange();		// ステージの切り替え

		bool BgmManage(std::string nextStage);
		std::string BgmKey(std::string key);	// キーの切り替え
	};

}
