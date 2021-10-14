/*****************************************************************//**
 * \file   ModeMain.h
 * \brief  �Q�[���{�҂��Ǘ����郂�[�h���C���N���X�i���[�h�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "Vector2.h"
#include "ImageServer.h"
#include <memory>

namespace inr {
	// ��d�C���N���[�h�h�~
	class Pause;
	class EffectServer;
	class StageUi;
	class ForeGround;
	class ItemServer;
	class TutorialServer;
	// �Q�[���{��
	class ModeMain : public ModeBase {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		ModeMain(Game& game);
		// �f�X�g���N�^
		~ModeMain();
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
		// �X�e�[�W�L�[�̐؂�ւ�(����:���̃X�e�[�W�L�[)
		void ChangeKey(const std::string nextStage) override;
		// �؂�ւ�
		bool IsKeyNull();

		bool GameOver();	// �t���O�ύX
		bool OpenBossStage();	// �{�X�X�e�[�W�ւ̔����������
		inline std::string StageKey() { return _stageKey; }
		inline bool BossOpen() { return _bossOpen; }
		inline bool BossFlag() { return _bossBattle; }
		bool StageReset(); // �X�e�[�W����߂��i���Q�[���I�[�o�[�j
		bool BossBattle();	// �{�X��J�n�t���O
		bool BossEnd();	// �{�X��I���t���O
		std::shared_ptr<EffectServer> GetEffectServer() { return _eServer; }
		std::unique_ptr<ImageServer>& GetItemImages() { return _messageServer; }
		std::unique_ptr<ItemServer>& GetItemServer() { return _itemServer; }
		std::unique_ptr<TutorialServer>& GetTutorialServer() { return _tutorialServer; }
	private:
		std::string _stageKey;	// �X�e�[�W�����p
		std::string _changeKey;		// �X�e�[�W�؂�ւ��悤
		std::unique_ptr<BackGround> _bg;	// �w�i
		std::unique_ptr<StageUi> _stageUi;
		// std::unique_ptr<UI> _uiSoul;
		std::unique_ptr<Pause> _pause;
		std::shared_ptr<EffectServer> _eServer;
		std::unique_ptr<ImageServer> _messageServer;
		std::unique_ptr<ForeGround> _fg;	// �O�i
		std::unique_ptr<ItemServer> _itemServer;
		std::unique_ptr<TutorialServer> _tutorialServer;

		bool _isReset;	// ���݂̃X�e�[�W�̏��������s�����H�i�j
		bool _bossOpen;	// �{�X�X�e�[�W�ւ̔��͊J����Ă��邩�H
		bool _bossBattle;	// �{�X��͍s���Ă��邩�H
		bool _isEnding;	// �G���f�B���O�Ɉڍs���邩
		int _endCount;	// �I������܂ł̑҂�����

		Vector2 _worldPosition;	// ���[���h���W

		bool IsStageChange();	// �X�e�[�W�̐؂�ւ����s�����H
		void SetObjects();	// �I�u�W�F�N�g�̐����i�Ĕz�u�j
		bool StageChange();		// �X�e�[�W�̐؂�ւ�

		bool BgmManage(std::string nextStage);
		std::string BgmKey(std::string key);	// �L�[�̐؂�ւ�
		bool IsEnding();
	};

}
