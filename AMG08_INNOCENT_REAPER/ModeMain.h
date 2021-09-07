#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "UI.h"
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

		void ChangeKey(const std::string nextStage) override;
		bool IsKeyNull();	// �L�[�͋󂩁H

		inline std::string StageKey() { return _stageKey; }
	private:
		std::string _stageKey;	// �X�e�[�W�����p
		std::string _changeKey;		// �X�e�[�W�؂�ւ��悤
		std::unique_ptr<BackGround> _bg;	// �w�i
		std::unique_ptr<UI> _uiSoul;

		Vector2 _worldPosition;	// ���[���h���W

		bool IsStageChange();	// �X�e�[�W�̐؂�ւ����s�����H
		void SetObjects();	// �I�u�W�F�N�g�̐����i�Ĕz�u�j
		bool StageChange();		// �X�e�[�W�̐؂�ւ�
	};

}
