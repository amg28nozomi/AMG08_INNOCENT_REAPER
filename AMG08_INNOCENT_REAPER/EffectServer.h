#pragma once
#include <vector>
#include <memory>

namespace inr {
	namespace effect {
		namespace type {
			// �G�t�F�N�g�̕`�揇��
			constexpr auto FORMER = 0;	// �I�u�W�F�N�g���O�ʂɕ`��
			constexpr auto BACK = 1;	// �I�u�W�F�N�g����ʂɕ`��
		}
	}
	// ��d�C���N���[�h�h�~
	class Game;	
	class EffectBase;
	// �G�t�F�N�g�̊Ǘ����s���T�[�o�N���X
	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// �z�񏉊���
		// ������
		void Init();
		// �X�V
		void Process();	
		// �O�`��
		void DrawFormer();
		// ��`��
		void DrawBack();
		// �G�t�F�N�g�̓o�^(����1:�o�^�G�t�F�N�g�@����2:�ǂ���ɓo�^���邩)
		void Add(std::unique_ptr<EffectBase> eff, int type);
		// �����t���O���I���ɂ���
		inline void DelOn() { if (_del != true) _del = true; }
	private:
		Game& _game;
		bool _isAdd;	// �V�K�G�t�F�N�g���ǉ����ꂽ���H
		bool _update;	// �X�V���s���Ă��邩�H
		bool _del;	// �������邩�ۂ�
		std::vector<std::unique_ptr<EffectBase>> _effectsF;	// �G�t�F�N�g��ԗp
		std::vector<std::unique_ptr<EffectBase>> _effectsB;

		std::vector<std::unique_ptr<EffectBase>> _addFormer;
		std::vector<std::unique_ptr<EffectBase>> _addBack;

		void Clears();	// ������
		void IsDelete();
		bool IsEmpty();
	};
}

