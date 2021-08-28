#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "Vector2.h"
#include <memory>

namespace inr {

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;

		inline std::string GetKey() { return _stageKey; }
		inline void ChangeKey(const std::string nextStage) override { _changeKey = nextStage; }

	private:
		std::string _stageKey;	// ステージ検索用
		std::string _changeKey;	
		std::unique_ptr<BackGround> _bg;	// 背景

		Vector2 _worldPosition;	// ワールド座標
	};

}
