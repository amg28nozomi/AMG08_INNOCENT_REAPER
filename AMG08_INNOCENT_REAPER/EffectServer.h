/*****************************************************************//**
 * @file   EffectServer.h
 * @brief  �G�t�F�N�g�T�[�o�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include <memory>

namespace inr {
	/** �G�t�F�N�g */
	namespace effect {
		/** ��� */
		namespace type {
			// �G�t�F�N�g�̕`�揇��
			constexpr auto FORMER = 0;	//!< �I�u�W�F�N�g���O�ʂɕ`��
			constexpr auto BACK = 1;		//!< �I�u�W�F�N�g����ʂɕ`��
		}
	}
	/** ��d�C���N���[�h�h�~ */
	class Game;	
	class EffectBase;
	/** �G�t�F�N�g�̊Ǘ����s���T�[�o�N���X */
	class EffectServer {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		EffectServer(Game& game);
		// �f�X�g���N�^
		~EffectServer();
		// ������
		void Init();
		// �X�V
		void Process();	
		// �O�i�`��
		void DrawFormer();
		// ��i�`��
		void DrawBack();
		// �G�t�F�N�g�̓o�^(����1:�o�^�G�t�F�N�g�@����2:�ǂ���ɓo�^���邩)
		void Add(std::unique_ptr<EffectBase> eff, int type);
		// �����t���O���I���ɂ���
		inline void DelOn() { if (_del != true) _del = true; }
	private:
		Game& _game;											// �Q�[���N���X�Q��
		bool _isAdd;											// �V�K�G�t�F�N�g���ǉ����ꂽ���H
		bool _update;											// �X�V���s���Ă��邩�H
		bool _del;												// �o�^�G�t�F�N�g�̏����������Ăяo�����̔���
		std::vector<std::unique_ptr<EffectBase>> _effectsF;		// �O�i�G�t�F�N�g�i�[�p�R���e�i
		std::vector<std::unique_ptr<EffectBase>> _effectsB;		// ��i�G�t�F�N�g�i�[�p�R���e�i(��`��)
		std::vector<std::unique_ptr<EffectBase>> _addFormer;	// �ꎞ�I�i�[�p�R���e�i(�O�i�p)
		std::vector<std::unique_ptr<EffectBase>> _addBack;		// �ꎞ�I�i�[�p�R���e�i(��i�p)
		// �e��R���e�i�̉������
		void Clears();
		// �o�^�G�t�F�N�g�̏�������
		void IsDelete();
		// �ꎞ�I�i�[�p�R���e�i�ɓo�^����Ă���G�t�F�N�g�̏��L�����Y������R���e�i�Ɉڍs����
		void MoveAddEffects();
	};
}

