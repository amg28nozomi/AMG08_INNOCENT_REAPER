/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  ���[�h�̃X�[�p�[�N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>

namespace inr {
	namespace mode {
		// �e�탂�[�h�̃L�[���
		constexpr auto TITLE = "mode_title";	// �^�C�g��
		constexpr auto MAIN = "mode_main";		// �{��
		constexpr auto FIN = "mode_end";		// �I��
	}
	// ��d�C���N���[�h�h�~
	class Game;
	class ObjectServer;
	class ModeServer;
	// ���[�h�x�[�X
	class ModeBase {
	protected:
		static int _modeFrame;	// �o�߃t���[��
		bool _update;			// �������X�V���邩�ǂ���
		bool _resetFlg;			// ���[�h�؂�ւ����ɏ��������邩�ǂ���
		Game& _game;			// �Q�[���N���X�Q��
		std::string _bgmKey;	// bgm�̃L�[
		// BGM�̊Ǘ�
		bool BgmManage();
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		ModeBase(Game& game);
		// �f�X�g���N�^
		~ModeBase();
		// ������
		virtual void Init();
		// �X�V
		virtual void Process();	
		// �`��
		virtual void Draw();
		// �X�e�[�W�L�[�̐؂�ւ�(����:���̃X�e�[�W�L�[)
		virtual void ChangeKey(const std::string nextStage);
		// �������t���O�̃I��
		inline void Reset() { _resetFlg = true; }
		// �o�߃t���[���̏�����
		inline void TimeClear() { _modeFrame = 0; };
		// �o�߃t���[�����擾
		inline int GetFrame() { return _modeFrame; }
		// ���[�h�̎擾
		virtual inline std::string GetKey() { return ""; }
	};
}