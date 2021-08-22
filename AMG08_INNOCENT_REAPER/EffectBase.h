#pragma once
#include "Game.h"
#include "Vector2.h"

namespace inr {

	namespace effect {
		constexpr auto NO_COUNT = 0;
		constexpr auto NO_MAXFRAME = 0;
	}

	class Game;

	// �G�t�F�N�g
	class EffectBase {
	public:
		EffectBase(Game& game, const Vector2 spawnpos);
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 
	private:
		Game& _game;	// �Q�[���Q��

		Vector2 _position;	// ���W
		std::pair<int, int> _count;	// ����:��������(�t���[��)�A�����\�t���[��
	};
}

