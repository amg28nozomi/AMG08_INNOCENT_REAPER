/*****************************************************************//**
 * @file   GimmickServer.h
 * @brief  �M�~�b�N�T�[�o�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	/**  ��d�C���N���[�h�h�~ */
	class Game;
	class GimmickBase;
	/** �M�~�b�N�̊Ǘ����s�� */
	class GimmickServer {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 * @param game		�Q�[���N���X�̎Q��
		 */
		GimmickServer(Game& game);
		/**
		 * @brief					�f�X�g���N�^
		 */
		~GimmickServer();
		/**
		 * @brief					�X�V����
		 */
		void Process();
		/**
		 * @brief					�`�揈��
		 */
		void Draw();	
		/**
		 * @brief					�M�~�b�N�̓o�^
		 * @param gimmick	�o�^�M�~�b�N�̃V�F�A�[�h�|�C���^
		 */
		void Add(std::shared_ptr<GimmickBase> gimmick);
		/**
		 * @brief					�M�~�b�N�̍폜
		 */
		void Del();
		/**
		 * @brief					�R���e�i�̊J��
		 */
		inline void Clear() { _gimmicks.clear(); }
		/**
		 * @brief					�����t���O���I���ɂ���
		 */
		inline void DelOn() { _isDel = true; }
		/**
		 * @brief					�S�M�~�b�N�̎擾
		 * @return				�M�~�b�N���o�^����Ă���R���e�i��Ԃ�
		 */
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }
	private:
		Game& _game;																					//!< �Q�[���N���X�̎Q��
		bool _isDel;																					//!< �I�u�W�F�N�g���������邩�ۂ�
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	//!< �M�~�b�N���Ǘ�����R���e�i
	};
}

