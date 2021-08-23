#pragma once
#include "ModeBase.h"

namespace inr {

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;
	private:
		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;
	};
}
