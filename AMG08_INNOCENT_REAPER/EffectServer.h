#pragma once
#include <vector>
#include <memory>

namespace inr {

	class Game;
	class EffectBase;

	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// �z�񏉊���

		void Init();	// ������
		void Process();	// �X�V
		void Draw();	// �`��

		void Add(std::unique_ptr<EffectBase> eff);
	private:
		Game& _game;
		bool _update;	// �X�V���s���Ă��邩�H
		std::vector<std::unique_ptr<EffectBase>> _effects;	// �G�t�F�N�g��ԗp
		std::vector<std::unique_ptr<EffectBase>> _addEffects;	// �ꎞ�ǉ��p

		void Clears();	// ������
		void IsDelete();
	};
}

