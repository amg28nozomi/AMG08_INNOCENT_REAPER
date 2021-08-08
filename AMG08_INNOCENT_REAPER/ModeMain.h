#pragma once
#include "ModeBase.h"
#include <string>

namespace inr {

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // 初期化処理(フラグがオンの場合はフレーム初期化)
		void Process() override;
		void Draw() override;
	private:
		std::string _stageKey;	// ステージ検索用
	};

}
