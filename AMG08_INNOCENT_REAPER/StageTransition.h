#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {

	class Game;
	// �J�ڌ�̃L�[�ƍ��W
	class Transition {
	public:
		Transition(std::string stageKey, Vector2 position);
		~Transition() = default;
	private:
		std::string _nextKey;	// �J�ڐ�̃L�[
		Vector2 _position;	// �J�ڌ�̍��W(���@)
	};

	// �X�e�[�W�J�ڎ��s�p
	class StageTransition {
	public:
		StageTransition(Game& game);
		~StageTransition();

		bool IsStageChange(const int no);	// �X�e�[�W�J�ڗp�`�b�v�ɐڐG�������H

		std::string StageChange(Vector2& _pos);
	private:
		Game& _game;	// �Q�[��

		void Clear();	// 
		std::unordered_map<int, Transition> _transitions;
	};
}

