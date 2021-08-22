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

	constexpr auto ADD_LUMINANCE = true;
	constexpr auto SUB_LUMINANCE = false;

	constexpr auto MAX_LUMINANCE = 255;
	constexpr auto MIN_LUMINANCE = 0;

	constexpr auto WAIT_TIME = 60;	// �ҋ@����
}

namespace inr {

	Logo::Logo(Game& game) : Image(game) {
		Init();
	}

	void Logo::Init() {
		_number = LOGO_FIRST;
		_calculation = ADD_LUMINANCE;	// ���Z�����ɐ؂�ւ�
		ChangeGraph();
	}

	void Logo::Process() {
		// A�{�^���̓��͂��������ꍇ�͏������X�L�b�v����
		if (_number != LOGO_THIRD && _game.GetTrgKey() == PAD_INPUT_3) {
			ChangeGraph();	// �A�j���[�V������؂�ւ�
		}
		// �ҋ@���Ԃ�����ꍇ�͌��Z�����������s���������甲����
		if (_wait) {
			--_wait;
			return;
		}
		// ModeMain�œ��͏������������ꍇ�A�������X�L�b�v����悤�ɕύX����
		// �����ŕϐ��̒l��ύX
		++_fCount;
		// �҂����Ԃ��Ȃ��ꍇ�͋P�x�̒l�ɕύX�������ďI��
		switch (_calculation) {
		case ADD_LUMINANCE:
			_pal += LUMINANCE;
			// �����𖞂������ꍇ��60�t���[���̊ԍX�V�������Ȃ��悤�ɂ���
			if (MAX_LUMINANCE <= _pal) {
				_pal = MAX_LUMINANCE;	// �l���C��
				++_number;
				_calculation = SUB_LUMINANCE;
				_wait = WAIT_TIME;
				ChangeGraph();
			}
			break;
		case SUB_LUMINANCE:
			_pal -= LUMINANCE;
			if (_pal <= MIN_LUMINANCE) {
				_calculation = SUB_LUMINANCE;
				_wait = WAIT_TIME;
			}
			break;
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int p = static_cast<int>(_pal);
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// �`�悷��O���t�B�b�N�n���h���̎擾
		SetDrawBlendMode(DX_BLENDMODE_SUB, p);
		DrawGraph(x, y, grh, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ���̕`��ɔ��f����Ȃ��悤�Ƀm�[�}���u�����h�ɖ߂�
	}

	void Logo::AnimationInit() {
		_pal = 0;
		_graphKey = TEAM_LOGO;	// �`�[�����S�ɐ؂�ւ�
	}

	void Logo::ChangeGraph() {
		_fCount = 0;
		_pal = 255;		
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
