#include "StageTransition.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "Game.h"

namespace {
	constexpr auto CHIP_NULL = -1;
}

namespace inr {

	namespace start {
		// 対応座標
		constexpr auto TPOS_0_X = 200;
		constexpr auto TPOS_0_Y = 1900;

		constexpr auto TPOS_1_X = 3750;
		constexpr auto TPOS_1_Y = 300;

		constexpr auto TPOS_2_X = 100;
		constexpr auto TPOS_2_Y = 1970;

		constexpr auto TPOS_4_X = 9500;
		constexpr auto TPOS_4_Y = 1970;
	}

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
		_transitions = {
			{ 129, {stage::STAGE_1, {start::TPOS_0_X, start::TPOS_0_Y}}},
			{ 130, {stage::STAGE_0, {start::TPOS_1_X, start::TPOS_1_Y}}},
			{ 131, {stage::STAGE_2, {start::TPOS_2_X, start::TPOS_2_Y}}},
			// { 132, {stage::STAGE_1, {}}
			{ 133, {stage::STAGE_1, {start::TPOS_4_X, start::TPOS_4_Y}}},

		};

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

	bool StageTransition::IsHit(const int no) {
		auto it = _transitions.find(no);
		if (it == _transitions.end()) return false;	// 接触していない
		_number = no;	// 接触したキーを記録

		auto modemain = _game.GetModeServer()->GetModeMain();
		if (modemain->IsKeyNull()) modemain->ChangeKey(it->second.NextKey());
		return true;// 接触した（ヒットした）
	}

	bool StageTransition::IsStageChange() {
		// -1ではない場合（値に更新がかかっている場合、更新を処理を行う）
		bool flag = _number == CHIP_NULL;
		return flag;
	}

	Vector2 StageTransition::SetPosition() {
		// 対応するキーと情報を付与する
		auto it = _transitions.find(_number);
		_number = CHIP_NULL;	// キーを空にする
		return it->second.NextPosition();
	}

	// どうやって自機が接触したかどうかの判定を行うのか
}