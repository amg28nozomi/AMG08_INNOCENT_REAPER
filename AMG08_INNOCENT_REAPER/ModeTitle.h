#pragma once
#include "ModeBase.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {

	class Logo;

	class ModeTitle : public ModeBase{
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		ModeTitle(Game& game);
		~ModeTitle();

		virtual void Init();	// ������
		virtual void Process();	// �X�V
		virtual void Draw();	// �`��

	private:

		std::unique_ptr<Logo> _titleGh;
	};
}