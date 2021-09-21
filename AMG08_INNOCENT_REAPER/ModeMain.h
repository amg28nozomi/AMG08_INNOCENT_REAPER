#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "UI.h"
#include "Vector2.h"
#include <memory>

namespace inr {

	class Pause;
	class EffectServer;
	class StageUi;

	class ModeMain : public ModeBase {
	public:
		ModeMain(Game& game);
		~ModeMain();

		void Init() override; // ����������(�t���O���I���̏ꍇ�̓t���[��������)
		void Process() override;
		void Draw() override;

		void ChangeKey(const std::string nextStage) override;
		bool IsKeyNull();	// �L�[�͋󂩁H

		bool GameOver();	// �t���O�ύX
		bool OpenBossStage();	// �{�X�X�e�[�W�ւ̔����������
		inline std::string StageKey() { return _stageKey; }
		inline bool BossOpen() { return _bossOpen; }
		inline bool BossFlag() { return _bossBattle; }
		bool StageReset(); // �X�e�[�W����߂��i���Q�[���I�[�o�[�j
		bool BossBattle();	// �{�X��J�n�t���O
		std::shared_ptr<EffectServer> GetEffectServer() { return _eServer; }
	private:
		std::string _stageKey;	// �X�e�[�W�����p
		std::string _changeKey;		// �X�e�[�W�؂�ւ��悤
		std::unique_ptr<BackGround> _bg;	// �w�i
		std::unique_ptr<StageUi> _stageUi;
		std::unique_ptr<UI> _uiSoul;
		std::unique_ptr<Pause> _pause;
		std::shared_ptr<EffectServer> _eServer;

		bool _isReset;	// ���݂̃X�e�[�W�̏��������s�����H�i�j
		bool _bossOpen;	// �{�X�X�e�[�W�ւ̔��͊J����Ă��邩�H
		bool _bossBattle;	// �{�X��͍s���Ă��邩�H

		Vector2 _worldPosition;	// ���[���h���W

		bool IsStageChange();	// �X�e�[�W�̐؂�ւ����s�����H
		void SetObjects();	// �I�u�W�F�N�g�̐����i�Ĕz�u�j
		bool StageChange();		// �X�e�[�W�̐؂�ւ�

		bool BgmManage(std::string nextStage);
		std::string BgmKey(std::string key);	// �L�[�̐؂�ւ�
	};

}
