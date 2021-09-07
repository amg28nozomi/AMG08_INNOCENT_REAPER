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
		if (it == _transitions.end()) return false;	// 接触していない
		return true;// 接触した（ヒットした）
	}

	std::string StageTransition::StageChange(Vector2& _pos) {
		// ステージ切り替え
	}

	// どうやって自機が接触したかどうかの判定を行うのか
}