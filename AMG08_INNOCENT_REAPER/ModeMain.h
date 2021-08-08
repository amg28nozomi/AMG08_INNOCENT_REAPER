#pragma once
#include "ModeBase.h"
#include <string>

namespace inr {

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;
	private:
		std::string _stageKey;	// �X�e�[�W�����p
	};

}
