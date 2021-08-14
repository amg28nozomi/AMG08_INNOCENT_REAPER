#pragma once
#include "ModeBase.h"
#include <memory>

namespace inr {

	class TitleLogo;

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// ‰Šú‰»
		virtual void Process();	// XV
		virtual void Draw();	// •`‰æ

	private:
		std::unique_ptr<TitleLogo> _titleLG;
	};
}