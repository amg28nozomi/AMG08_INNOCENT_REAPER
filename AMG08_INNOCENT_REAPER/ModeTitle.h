#pragma once
#include "ModeBase.h"

namespace inr {

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// ‰Šú‰»
		virtual void Process();	// XV
		virtual void Draw();	// •`‰æ

	private:

	};
}