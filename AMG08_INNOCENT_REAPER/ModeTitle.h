#pragma once
#include "ModeBase.h"

namespace inr {

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// 初期化
		virtual void Process();	// 更新
		virtual void Draw();	// 描画

	private:

	};
}