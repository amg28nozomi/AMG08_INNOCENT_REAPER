#pragma once
#include "ModeBase.h"

namespace inr {

	class ModeTitle : public ModeBase{
	public:
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// ������
		virtual void Process();	// �X�V
		virtual void Draw();	// �`��

	private:

	};
}