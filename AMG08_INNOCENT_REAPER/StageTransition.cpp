#include "StageTransition.h"
#include "Game.h"

namespace inr {

	Transition::Transition(std::string stageKey, Vector2 position) {
		_nextKey = stageKey;
		_position = position;
	}

	StageTransition::StageTransition(Game& game) : _game(game) {
		Clear();
	}

	StageTransition::~StageTransition() {
		Clear();
	}

	void StageTransition::Clear() {
		_transitions.clear();
	}

	bool StageTransition::IsStageChange(const int no) {
		auto it = _transitions.find(no);
		if (it == _transitions.end()) return false;	// �ڐG���Ă��Ȃ�
		return true;// �ڐG�����i�q�b�g�����j
	}

	std::string StageTransition::StageChange(Vector2& _pos) {
		// �X�e�[�W�؂�ւ�
	}

	// �ǂ�����Ď��@���ڐG�������ǂ����̔�����s���̂�
}