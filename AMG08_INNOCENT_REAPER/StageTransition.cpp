#include "StageTransition.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "Game.h"

namespace {
	constexpr auto CHIP_NULL = -1;
}

namespace inr {

	namespace start {
		// �Ή����W
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
		// �Ή�����`�b�v�̓����蔻���ݒ肷��
		_transitions = {
			{ 129, {stage::STAGE_1, {start::TPOS_0_X, start::TPOS_0_Y}}},
			{ 130, {stage::STAGE_0, {start::TPOS_1_X, start::TPOS_1_Y}}},
			{ 131, {stage::STAGE_2, {start::TPOS_2_X, start::TPOS_2_Y}}},
			// { 132, {stage::STAGE_1, {}}
			{ 133, {stage::STAGE_1, {start::TPOS_4_X, start::TPOS_4_Y}}},

		};

		/*
		{ 129, {mapchip::TRANSITION}},	// �X�e�[�WS(1)���X�e�[�W1(2)
			{ 130, {mapchip::TRANSITION}},	// �X�e�[�W1(3)���X�e�[�W2-2()
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
		if (it == _transitions.end()) return false;	// �ڐG���Ă��Ȃ�
		_number = no;	// �ڐG�����L�[���L�^

		auto modemain = _game.GetModeServer()->GetModeMain();
		if (modemain->IsKeyNull()) modemain->ChangeKey(it->second.NextKey());
		return true;// �ڐG�����i�q�b�g�����j
	}

	bool StageTransition::IsStageChange() {
		// -1�ł͂Ȃ��ꍇ�i�l�ɍX�V���������Ă���ꍇ�A�X�V���������s���j
		bool flag = _number == CHIP_NULL;
		return flag;
	}

	Vector2 StageTransition::SetPosition() {
		// �Ή�����L�[�Ə���t�^����
		auto it = _transitions.find(_number);
		_number = CHIP_NULL;	// �L�[����ɂ���
		return it->second.NextPosition();
	}

	// �ǂ�����Ď��@���ڐG�������ǂ����̔�����s���̂�
}