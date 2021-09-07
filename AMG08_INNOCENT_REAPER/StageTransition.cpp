#include "StageTransition.h"
#include "Game.h"

namespace {
	constexpr auto CHIP_NULL = -1;
}

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

	bool StageTransition::Init() {
		// 対応するチップの当たり判定を設定する
		/*_transitions = {
			{129}, 
		};*/

		/*
		{ 129, {mapchip::TRANSITION}},	// ステージS(1)→ステージ1(2)
			{ 130, {mapchip::TRANSITION}},	// ステージ1(3)→ステージ2-2()
			{ 131, {mapchip::TRANSITION}},
			{ 132, {mapchip::TRANSITION}},
			{ 133, {mapchip::TRANSITION}},
			{ 134, {mapchip::TRANSITION}},
			{ 135, {mapchip::TRANSITION}},
			{ 136, {mapchip::TRANSITION}},
			{ 137, {mapchip::TRANSITION}},
			{ 138, {mapchip::TRANSITION}},
			{ 139, {mapchip::TRANSITION}},
			{ 140, {mapchip::TRANSITION}},
			{ 141, {mapchip::TRANSITION}},
			{ 142, {mapchip::TRANSITION}},
			{ 143, {mapchip::TRANSITION}},
			{ 144, {mapchip::TRANSITION}},
		*/
		return true;
	}

	bool StageTransition::IsStageChange(const int no) {
		auto it = _transitions.find(no);
		if (it == _transitions.end()) return false;	// 接触していない
		_number = no;	// 接触したキーを記録
		return true;// 接触した（ヒットした）
	}

	std::string StageTransition::StageChange(Vector2& _pos) {
		// 対応するキーと情報を付与する
		auto it = _transitions.find(_number);
		_pos = it->second.NextPosition();	//	ポジションを更新
		_number = CHIP_NULL;	// キーを空にする
		return it->second.NextKey();
	}

	// どうやって自機が接触したかどうかの判定を行うのか
}