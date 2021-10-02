#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {

	class ModeEnd : public ModeBase {
	public:
		ModeEnd(Game& game);
		~ModeEnd() = default;

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;
	private:
		int _count;	// �҂�����
		bool _end;	// �������I�����邩�H
		bool _input; // ���͂��󂯕t���邩�H
		std::unique_ptr<MoveImage> _staffRoll;

		bool IsEnd();	// 
		bool IsInput();
	};
}

