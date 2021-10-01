#pragma once
#include "ModeBase.h"

namespace inr {

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;
	private:
		bool _end;	// �������I�����邩�H
		std::unique_ptr<>

		bool IsEnd();	// 
	};
}

