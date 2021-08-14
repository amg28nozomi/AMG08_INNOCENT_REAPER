#pragma once
#include "ModeBase.h"
#include <memory>

namespace inr {

	class TitleLogo;

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// ������
		virtual void Process();	// �X�V
		virtual void Draw();	// �`��

	private:
		std::unique_ptr<TitleLogo> _titleLG;
	};
}