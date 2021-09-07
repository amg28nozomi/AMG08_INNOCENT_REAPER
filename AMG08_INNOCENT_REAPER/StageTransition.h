#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {

	class Game;
	// 遷移後のキーと座標
	class Transition {
	public:
		Transition(std::string stageKey, Vector2 position);
		~Transition() = default;
	private:
		std::string _nextKey;	// 遷移先のキー
		Vector2 _position;	// 遷移後の座標(自機)
	};

	// ステージ遷移実行用
	class StageTransition {
	public:
		StageTransition(Game& game);
		~StageTransition();

		bool IsStageChange(const int no);	// ステージ遷移用チップに接触したか？

		std::string StageChange(Vector2& _pos);
	private:
		Game& _game;	// ゲーム

		void Clear();	// 
		std::unordered_map<int, Transition> _transitions;
	};
}

