#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "Vector2.h"
#include <memory>

namespace inr {

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;

		inline std::string GetKey() { return _stageKey; }
		inline void ChangeKey(const std::string nextStage) override { _changeKey = nextStage; }

	private:
		std::string _stageKey;	// �X�e�[�W�����p
		std::string _changeKey;	
		std::unique_ptr<BackGround> _bg;	// �w�i

		Vector2 _worldPosition;	// ���[���h���W
	};

}
