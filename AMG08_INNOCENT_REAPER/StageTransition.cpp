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
		// �Ή�����`�b�v�̓����蔻���ݒ肷��
		/*_transitions = {
			{129}, 
		};*/

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

	bool StageTransition::IsStageChange(const int no) {
		auto it = _transitions.find(no);
		if (it == _transitions.end()) return false;	// �ڐG���Ă��Ȃ�
		_number = no;	// �ڐG�����L�[���L�^
		return true;// �ڐG�����i�q�b�g�����j
	}

	std::string StageTransition::StageChange(Vector2& _pos) {
		// �Ή�����L�[�Ə���t�^����
		auto it = _transitions.find(_number);
		_pos = it->second.NextPosition();	//	�|�W�V�������X�V
		_number = CHIP_NULL;	// �L�[����ɂ���
		return it->second.NextKey();
	}

	// �ǂ�����Ď��@���ڐG�������ǂ����̔�����s���̂�
}