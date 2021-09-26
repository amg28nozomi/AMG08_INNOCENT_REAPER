#pragma once
#include <vector>
#include <memory>

namespace inr {

	namespace effect {
		namespace type {
			constexpr auto FORMER = 0;
			constexpr auto BACK = 1;
		}
	}

	class Game;
	class EffectBase;

	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// �z�񏉊���

		void Init();	// ������
		void Process();	// �X�V
		void DrawFormer();	// �`��
		void DrawBack();

		void Add(std::unique_ptr<EffectBase> eff, int type);	// ����1:�G�t�F�N�g�@����2:�ǂ���ɓo�^���邩
		inline void DelOn() { if (_del != true) _del = true; }	// �����t���O���I���ɂ���
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

