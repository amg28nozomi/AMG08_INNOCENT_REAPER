#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {

	namespace stage {
		// ステージ読み込み用のキー
		constexpr auto CHANGE_NULL = "change_null";	// 通常状態のキー切り替え

		constexpr auto STAGE_0 = "stage0";	// チュートリアル
		constexpr auto STAGE_1 = "stage1";		// ステージ1(廃村)
		constexpr auto STAGE_2 = "stage2";	// ステージ2(森林-中央)
		constexpr auto STAGE_2_1 = "stage2-1";	// ステージ2(森林-上部)
		constexpr auto STAGE_2_2 = "stage2-2";	// ステージ2(森林-下部)
		constexpr auto STAGE_3 = "boss_stage";	// ステージ3(ボス戦)
	}

	class Game;
	// 遷移後のキーと座標
	class Transition {
	public:
		Transition(std::string stageKey, Vector2 position);
		~Transition() = default;

		inline std::string NextKey() { return _nextKey; }
		inline Vector2 NextPosition() { return _position; }
	private:
		std::string _nextKey;	// 遷移先のキー
		Vector2 _position;	// 遷移後の座標(自機)
	};

	// ステージ遷移実行用
	class StageTransition {
	public:
		StageTransition(Game& game);
		~StageTransition();

		bool Init();
		bool IsStageChange(const int no);	// ステージ遷移用チップに接触したか？


		std::string StageChange(Vector2& _pos);
	private:
		Game& _game;	// ゲーム
		int _number;	// 接触したチップ

		void Clear();	// 
		std::unordered_map<int, Transition> _transitions;
	};
}

