/*****************************************************************//**
 * @file   BackGround.cpp
 * @brief  �w�i�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include "MapChips.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include <DxLib.h>

namespace {
	// �X�N���[�����x
	constexpr auto BACKGROUND_SPEED = 0.5;
	// �X�N���[�����
	constexpr auto MAX_SCROLL = 1920 + 960;
	// �X�N���[������
	constexpr auto MIN_SCROLL = -960;
	// �X�e�[�W1�̊e��X�N���[�����x
	constexpr auto STAGE1_BACK_00 = 0.30;
	constexpr auto STAGE1_BACK_01 = 0.4;
	constexpr auto STAGE1_BACK_02 = 0.6;
	// �X�e�[�W2�̊e��X�N���[�����x
	constexpr auto STAGE2_BACK_00 = 0.25;
	constexpr auto STAGE2_BACK_01 = 0.40;
	constexpr auto STAGE2_BACK_02 = 0.50;
	constexpr auto STAGE2_BACK_03 = 0.60;
}

namespace inr {
	// �R���X�g���N�^
	BackGround::BackGround(Game& game) : Image(game){
		// �e��ϐ�������
		_stageNo = stage::number::SN_NULL;
		_pos = { 960 , 1080 };
		_fix = { 0, 0 };
		_scroll = true;	// �X�N���[���I��
	}
	// ������
	void BackGround::Init() {
		// �w�i�摜�������A���������s��
		for (int n = 0; n < static_cast<int>(_scrSpeed.first.size()); ++n) {
			_positions.first.emplace_back(HALF_WINDOW_W, _fix.first);
			_positions.second.emplace_back(MAX_SCROLL, _fix.second);
		}
		ScrollY();	// �C����������
	}
	// �X�V
	void BackGround::Process() {
		ChangeGraph();	// �w�i�摜��؂�ւ��邩
		// �摜�L�[�ɉ����ČĂяo���X�N���[��������ύX
		if (_graphKey == background::BACK_GROUND_B) NormalManage();
		else BigManage();
	}
	// �`��
	void BackGround::Draw() {
		// �w�i�摜�������̕`�揈�������s����
		for (auto number = 0; number < static_cast<int>(_positions.first.size()); ++number) {
			auto x1 = _positions.first[number].IntX();
			auto y1 = _positions.first[number].IntY();

			auto x2 = _positions.second[number].IntX();
			auto y2 = _positions.second[number].IntY();
			auto gh = graph::ResourceServer::GetHandles(_graphKey, number);
			DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
			DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
		}
		// �����̃X�e�[�W��
		if (_game.GetModeServer()->GetModeMain()->StageKey() == stage::STAGE_0) BackDraw();
	}
	// �X�e�[�W�ɉ������摜�ɐ؂�ւ���i����:���݂̃X�e�[�W�j
	void BackGround::ChangeGraph() {
		// �X�e�[�W�͐؂�ւ�������H
		if (IsChanege() != true) return;
		// �؂�ւ�����ꍇ�͊e��ϐ��̗v�f���������
		_graphKey.clear();
		_scrSpeed.first.clear();
		_scrSpeed.second.clear();
		_positions.first.clear();
		_positions.second.clear();
		// ���݂̃X�e�[�W�ɉ����Ċe��l��o�^
		switch (_stageNo) {
		case stage::number::SN_S:	// �X�e�[�WS
			_graphKey = background::BACK_GROUND_S;
			_scrSpeed.first = { 0.5, 0.35 };
			_scrSpeed.second = { 0.75, 0.75 };
			_fix = { 1080 , 1080 };
			break;
		case stage::number::SN_T:	// �`���[�g���A��
			_graphKey = background::BACK_GROUND_S;
			_scrSpeed.first = { 0.5 };
			_scrSpeed.second = { 0.75 };
			_fix = { 0, 0 };
			break;
		case stage::number::SN_1:	// �X�e�[�W1
			_graphKey = background::BACK_GROUND_1;
			_scrSpeed.first = { STAGE1_BACK_00, STAGE1_BACK_01, STAGE1_BACK_02 };
			_scrSpeed.second = { STAGE1_BACK_00, STAGE1_BACK_01, STAGE1_BACK_02 };
			_fix = { 0, 0};
			break;
		case stage::number::SN_2:	// �X�e�[�W2
			_graphKey = background::BACK_GROUND_2;
			_scrSpeed.first = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_scrSpeed.second = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_fix = { 0, 0 };
			break;
		case stage::number::SN_B:		// �{�X�X�e�[�W
			_graphKey = background::BACK_GROUND_B;
			_scrSpeed.first = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_scrSpeed.second = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_fix = { 540, 540 };
			break;
		default:	// �Y���Ȃ�
#ifdef _DEBUG
			OutputDebugString("BackGround->ChangeGraph(const int stageNo) �Ώۂ̃X�e�[�W�͑��݂��܂���B\n");
#endif
			return;
		}
		Init();	// ��������������
	}
	// �L�[�͐؂�ւ�������H
	bool BackGround::IsChanege() {
		auto no = KeyNumber();	// �摜�ԍ����擾
		// �ԍ����������ꍇ�͏����𒆒f
		if (_stageNo == no) return false;
		// �Ⴄ�ꍇ�͍X�V
		_stageNo = no;
		return true;
	}
	// �X�e�[�W�ԍ��̔���
	int BackGround::KeyNumber() {
		// �X�e�[�W�L�[���擾
		auto skey = _game.GetModeServer()->GetModeMain()->StageKey();
		if (skey == stage::STAGE_0) return stage::number::SN_S;
		if (skey == stage::STAGE_T) return stage::number::SN_T;
		if (skey == stage::STAGE_1) return stage::number::SN_1;
		if (skey == stage::STAGE_2) return stage::number::SN_2;
		if (skey == stage::STAGE_2_1) return stage::number::SN_2;
		if (skey == stage::STAGE_2_2) return stage::number::SN_2;
		if (skey == stage::STAGE_3) return stage::number::SN_B;
		return -1;
	}
	// �O�`��
	void BackGround::BackDraw() {
		Vector2 xy = _pos;	// ���݂̍��W
		_game.GetMapChips()->Clamp(xy);	// �X�N���[�����W�ɃN�����v����
		// �`����W�̎Z�o
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		auto gh = graph::ResourceServer::GetHandles(background::ALTAR);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);	// �`��
	}
	// �X�N���[������(��)
	void BackGround::BigManage() {
		// ���[���h���W�̈ړ��ʂ��擾
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto moveY = _game.GetMapChips()->BeforeWorldPos().GetY() * -1;
		// �摜�������������s��
		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ���[���hX���W�̓X�N���[���J�n�n�_�𒴂��Ă��邩�H
			if (_game.GetMapChips()->IsScrollX() == true) {
				// �ړ��ʕ��������W�����炷
				_positions.first[i].GetPX() += moveX * _scrSpeed.first[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed.first[i];
				// �ꖇ�ڂ̏C������
				if (_positions.first[i].IntX() < -HALF_WINDOW_W) {
					// �R�ꂽ�l���Z�o
					auto vec = -HALF_WINDOW_W - _positions.first[i].GetX();
					_positions.first[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// �񖇖ڂ̏C������
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}
			// Y���W�̏C��
			if (_game.GetMapChips()->IsScrollY() == true) {
				// �ړ��ʕ��������W���ړ�������
				_positions.first[i].GetPY() += moveY * _scrSpeed.second[i];
				_positions.second[i].GetPY() += moveY * _scrSpeed.second[i];
				// ��������������ꍇ�͒l���C��
				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				// ������������ꍇ�͒l���C��
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}
	// �X�N���[������(��)
	void BackGround::NormalManage() {
		// �X�N���[���������I�t�̏ꍇ�A�������s��Ȃ�
		if (_scroll != true) return;
		// �ړ���X�̎擾
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		// �摜�������������s��
		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ���[���hX���W�̓X�N���[���J�n�n�_�𒴂��Ă��邩�H
			if (_game.GetMapChips()->IsScrollX() == true) {
				// �ړ��ʕ��������W�����炷
				_positions.first[i].GetPX() += moveX * _scrSpeed.first[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed.first[i];
				// �ꖇ�ڂ̏C������
				if (_positions.first[i].IntX() < -HALF_WINDOW_W) {
					// �R�ꂽ�l���Z�o
					auto vec = -HALF_WINDOW_W - _positions.first[i].GetX();
					_positions.first[i].GetPX() = MAX_SCROLL - vec;
				} else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// �񖇖ڂ̏C������
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				} else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}
		}
	}
	// Y���W�̏C��
	void BackGround::ScrollY() {
		// Y���W�̏C��
		for (auto i = 0; i < _positions.first.size(); ++i) {
			// �X�N���[�����W�𒴂��Ă��邩�H
			if (_game.GetMapChips()->IsScrollY() == true) {
				// ���W����
				_positions.first[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();
				_positions.second[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();
				// ��������������ꍇ�͏C��
				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				// ������������ꍇ���C��
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}
}