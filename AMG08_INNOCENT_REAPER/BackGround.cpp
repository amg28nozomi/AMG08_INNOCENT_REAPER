#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include "MapChips.h"
#include <DxLib.h>

namespace {
	constexpr auto BACKGROUND_SPEED = 0.5;
	constexpr auto MAX_SCROLL = 1920 + 960;
	constexpr auto MIN_SCROLL = -960;



	constexpr auto STAGE1_BACK_00 = 0.30;
	constexpr auto STAGE1_BACK_01 = 0.4;
	constexpr auto STAGE1_BACK_02 = 0.6;

	constexpr auto STAGE2_BACK_00 = 0.25;
	constexpr auto STAGE2_BACK_01 = 0.40;
	constexpr auto STAGE2_BACK_02 = 0.50;
	constexpr auto STAGE2_BACK_03 = 0.60;
}

namespace inr {

	BackGround::BackGround(Game& game) : Image(game){
		ChangeGraph(stage::number::SN_1);	// �ŏ��̃L�[��ǂݍ���
	}

	void BackGround::Init() {
		// �X�N���[�����W�̃T�C�Y���������������s��
		for (int n = 0; n < static_cast<int>(_scrSpeed.size()); ++n) {
			_positions.first.emplace_back(HALF_WINDOW_W, 0);
			_positions.second.emplace_back(MAX_SCROLL, 0);
		}
	}

	void BackGround::Process() {
		// ���[���h���W�̈ړ��ʂ��擾
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto moveY = _game.GetMapChips()->BeforeWorldPos().GetY() * -1;
		// auto moveY = _game.GetMapChips()->GetWorldVector().IntY() * -1;


		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ���[���hX���W�̓X�N���[���J�n�n�_�𒴂��Ă��邩�H
			if (_game.GetMapChips()->IsScrollX() == true) {
				// �ړ��ʕ��������W�����炷
				_positions.first[i].GetPX() += moveX * _scrSpeed[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed[i];
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

			if (_game.GetMapChips()->IsScrollY() == true) {
				_positions.first[i].GetPY() += moveY * _scrSpeed[i];
				_positions.second[i].GetPY() += moveY * _scrSpeed[i];

				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}

	void BackGround::Draw() {
		for (auto number = 0; number < static_cast<int>(_positions.first.size()); ++number) {
			auto x1 = _positions.first[number].IntX();
			auto y1 = _positions.first[number].IntY();

			auto x2 = _positions.second[number].IntX();
			auto y2 = _positions.second[number].IntY();
			auto gh = graph::ResourceServer::GetHandles(_graphKey, number);
			DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
			DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
			DrawFormatString(500, number * 100, GetColor(255, 0, 255), "backGround_y : %d\n", _positions.second[number].IntY());
		}
	}

	void BackGround::ChangeGraph(const int stageNo) {
		_scrSpeed.clear();	// ���g����ɂ���
		// ���݂̃X�e�[�W�ɉ����Ċe��l���X�V
		switch (stageNo) {
		case stage::number::SN_S:	// �X�e�[�WS
			_graphKey = stage::STAGE_0;
			_scrSpeed = { 0, 0, 0, 0, 0, 0, 0 };
			break;
		case stage::number::SN_1:	// �X�e�[�W1
			_graphKey = background::BACK_GROUND_1;
			_scrSpeed = { STAGE1_BACK_00, STAGE1_BACK_01, STAGE1_BACK_02 };
			break;
		case stage::number::SN_2:	// �X�e�[�W2
			_graphKey = background::BACK_GROUND_2;
			_scrSpeed = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			break;
		case stage::number::SN_B:		// �{�X�X�e�[�W
			break;
		default:	// �Y���Ȃ�
#ifdef _DEBUG
			OutputDebugString("BackGround->ChangeGraph(const int stageNo) �Ώۂ̃X�e�[�W�͑��݂��܂���B\n");
#endif
			return;
		}
		Init();	// ��������������
	}
}
