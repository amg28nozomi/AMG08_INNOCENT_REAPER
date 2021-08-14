#pragma once
#include "ModeBase.h"
#include <memory>

namespace inr {

	class TitleLogo;

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// 初期化
		virtual void Process();	// 更新
		virtual void Draw();	// 描画

	private:
		std::unique_ptr<TitleLogo> _titleLG;
	};
}