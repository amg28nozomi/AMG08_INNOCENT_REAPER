#include "StageTransition.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "Game.h"

namespace {
	constexpr auto CHIP_NULL = -1;
	constexpr auto CHIP_1 = 129;
	constexpr auto CHIP_2 = 130;
	constexpr auto CHIP_3 = 131;
	constexpr auto CHIP_4 = 132;
	constexpr auto CHIP_5 = 133;
	constexpr auto CHIP_6 = 134;
	constexpr auto CHIP_7 = 135;
	constexpr auto CHIP_8 = 136;
	constexpr auto CHIP_9 = 137;
	constexpr auto CHIP_10 = 138;
	constexpr auto CHIP_11 = 139;
	constexpr auto CHIP_12 = 140;
	constexpr auto CHIP_13 = 141;
	constexpr auto CHIP_14 = 142;
	constexpr auto CHIP_15 = 143;
	constexpr auto CHIP_16 = 144;
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

		constexpr auto TPOS_3_X = 100;
		constexpr auto TPOS_3_Y = 585;

		constexpr auto TPOS_4_X = 9500;
		constexpr auto TPOS_4_Y = 1970;

		constexpr auto TPOS_5_X = 9500;
		constexpr auto TPOS_5_Y = 585;

		constexpr auto TPOS_6_X = 1770;
		constexpr auto TPOS_6_Y = 1985;

		constexpr auto TPOS_7_X = 2120;
		constexpr auto TPOS_7_Y = 185;

		constexpr auto TPOS_8_X = 7820;
		constexpr auto TPOS_8_Y = 145;

		constexpr auto TPOS_9_X = 8030;
		constexpr auto TPOS_9_Y = 1985;

		constexpr auto TPOS_10_X = 1960;
		constexpr auto TPOS_10_Y = 100;

		constexpr auto TPOS_11_X = 5500;
		constexpr auto TPOS_11_Y = 1985;

		constexpr auto TPOS_12_X = 100;
		constexpr auto TPOS_12_Y = 800;

		constexpr auto TPOS_13_X = 9500;
		constexpr auto TPOS_13_Y = 1265;
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
			{ CHIP_1, {stage::STAGE_1, {start::TPOS_0_X, start::TPOS_0_Y}}},	// (1)→(2)
			{ CHIP_2, {stage::STAGE_0, {start::TPOS_1_X, start::TPOS_1_Y}}},	// (2)→(1)
			{ CHIP_3, {stage::STAGE_2, {start::TPOS_2_X, start::TPOS_2_Y}}},	// (3)→(5)
			{ CHIP_4, {stage::STAGE_2, {start::TPOS_3_X, start::TPOS_3_Y}}},	// (4)→(6)
			{ CHIP_5, {stage::STAGE_1, {start::TPOS_4_X, start::TPOS_4_Y}}},	// (5)→(3)
			{ CHIP_6, {stage::STAGE_1, {start::TPOS_5_X, start::TPOS_5_Y}}},	// (6)→(4)
			{ CHIP_7, {stage::STAGE_2_1, {start::TPOS_6_X, start::TPOS_6_Y}}},	// (7)→(8)
			{ CHIP_8, {stage::STAGE_2, { start::TPOS_7_X, start::TPOS_7_Y}}},	// (8)→(7)
			{ CHIP_9, {stage::STAGE_2, { start::TPOS_8_X, start::TPOS_8_Y}}},	// (9)→(10)
			{ CHIP_10, {stage::STAGE_2_1, { start::TPOS_9_X, start::TPOS_9_Y}}},	// (10)→(9)
			{ CHIP_11, {stage::STAGE_2_2, { start::TPOS_10_X, start::TPOS_10_Y}}},	// (11)→(12)
			{ CHIP_12, {stage::STAGE_2, { start::TPOS_11_X, start::TPOS_11_Y}}},	// (12)→(11)
			{ CHIP_13, {stage::STAGE_3, { start::TPOS_12_X, start::TPOS_12_Y}}},	// (13)→(14)
			{ CHIP_14, {stage::STAGE_2, { start::TPOS_13_X, start::TPOS_13_Y}}},	// (14)→(13)
			// { CHIP_15, {, { ,}}},	// ()→()
			// { CHIP_16, {, { ,}}},	// ()→()
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