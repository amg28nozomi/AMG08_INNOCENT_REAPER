#include "Logo.h"
#include "ResourceServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto LOGO_FIRST = 0;
	constexpr auto LOGO_SECOND = 1;// SECOND
	constexpr auto LOGO_THIRD = 2; // third
	constexpr auto LOGO_MAX = 3;

	constexpr auto LUMINANCE = 255 / 60;

	constexpr auto WAIT_TIME = 120;	// �ҋ@����
}

namespace inr {

	Logo::Logo(Game& game) : Image(game), _rgb() {
		Init();
	}

	void Logo::Init() {
		_wait = 0;
		_number = LOGO_FIRST;
		_calculation = rgb::ADD;	// ���Z�����ɐ؂�ւ�
		ChangeGraph();
	}

	void Logo::Process() {
		if (_number < LOGO_MAX) {
			// A�{�^���̓��͂��������ꍇ�͏������X�L�b�v����
			if (_number != LOGO_THIRD && _game.GetTrgKey() == PAD_INPUT_3) {
				++_number;
				_calculation = rgb::ADD;
				ChangeGraph();	// �A�j���[�V������؂�ւ�
			}
			// �ҋ@���Ԃ�����ꍇ�͌��Z�����������s���������甲����
			if (_wait) {
				--_wait;
				if (_wait == 0) _fCount = 0;
				return;
			}
			// ModeMain�œ��͏������������ꍇ�A�������X�L�b�v����悤�ɕύX����
			// �����ŕϐ��̒l��ύX
			++_fCount;
			// �҂����Ԃ��Ȃ��ꍇ�͋P�x�̒l�ɕύX�������ďI��
			switch (_calculation) {
			case rgb::ADD:
				// �����𖞂������ꍇ��60�t���[���̊ԍX�V�������Ȃ��悤�ɂ���
				if (_fCount == 60) {
					_calculation = rgb::SUB;
					_wait = WAIT_TIME;
					return;
				}
				_rgb.Update(_calculation, LUMINANCE);
				break;
			case rgb::SUB:
				if (_fCount == 60) {
					++_number;
					_calculation = rgb::ADD;
					ChangeGraph();
					return;
				}
				_rgb.Update(_calculation, LUMINANCE);
				break;
			}
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// �`�悷��O���t�B�b�N�n���h���̎擾
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());
		DrawRotaGraph(x, y, 1.0, 0, grh, true, false);
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);
	}

	void Logo::AnimationInit() {
		_graphKey = TEAM_LOGO;	// �`�[�����S�ɐ؂�ւ�
	}

	void Logo::ChangeGraph() {
		_fCount = 0;
		_rgb.Min();
		// ���݂̃i���o�[�ɉ����ăL�[��؂�ւ���
		switch (_number) {
		case LOGO_FIRST:	// AMG���S�ɑJ��
			_graphKey = AMG_LOGO;
			return;
		case LOGO_SECOND:	// �`�[�����S�ɑJ��
			_graphKey = TEAM_LOGO;
			return;
		case LOGO_THIRD:	// �^�C�g�����S�ɑJ��
			_graphKey = TITLE_LOGO;
			return;
		case LOGO_MAX:
			return;
		}
	}
}
