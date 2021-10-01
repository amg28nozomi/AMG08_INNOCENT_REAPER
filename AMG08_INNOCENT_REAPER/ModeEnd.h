#pragma once
#include "ModeBase.h"
#include <memory>

namespace inr {

	class MoveImage;

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;
	private:
		bool _end;	// �������I�����邩�H
		std::unique_ptr<MoveImage> _stuffRoll;

		bool IsEnd();	// 
	};
}

