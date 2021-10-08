/*****************************************************************//**
 * \file   GimmickServer.h
 * \brief  �M�~�b�N�T�[�o�N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	// ��d�C���N���[�h�΍�
	class Game;
	class GimmickBase;
	// �M�~�b�N�T�[�o
	// �M�~�b�N�x�[�X�I�u�W�F�N�g�̊Ǘ����s��
	class GimmickServer {
	public:
		GimmickServer(Game& game);
		~GimmickServer();
		// �X�V
		void Process();
		// �`��
		void Draw();	
		// �M�~�b�N�̓o�^(����1:�M�~�b�N)
		void Add(std::shared_ptr<GimmickBase> gimmick);
		// �M�~�b�N�̍폜
		void Del();
		// �M�~�b�N���o�^����Ă���R���e�i���擾
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }
		// �R���e�i�̉��
		inline void Clear() { _gimmicks.clear(); }
		// �����t���O��^�ɂ���
		inline void DelOn() { _isDel = true; }
	private:
		Game& _game;											// �Q�[���Q��
		bool _isDel;											// �I�u�W�F�N�g���������邩�ۂ�
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	// �M�~�b�N���Ǘ�����R���e�i
	};
}

