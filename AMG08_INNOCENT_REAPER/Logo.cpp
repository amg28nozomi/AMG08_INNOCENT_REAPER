#include "Logo.h"
#include "ResourceServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto LOGO_FIRST = 0;
	constexpr auto LUMINANCE = 255 / 60;
}

namespace inr {

	Logo::Logo(Game& game) : Image(game) {
		Init();
	}

	void Logo::Init() {
		_graphKey = AMG_LOGO;
		_pal = 0;
		_number = 0;
	}

	void Logo::Process() {
		// ModeMain�œ��͏������������ꍇ�A�������X�L�b�v����悤�ɕύX����
		// �����ŕϐ��̒l��ύX
		
		// A�{�^���̓��͂��������ꍇ�͏������X�L�b�v����
		if (_game.GetTrgKey() == PAD_INPUT_3) {
			AnimationInit();	// �A�j���[�V������ύX����
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// �`�悷��O���t�B�b�N�n���h���̎擾
		// SetDrawBlendMode(DX_BLENDMODE_SUB, _pal);
		DrawGraph(x, y, grh, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ���̕`��ɔ��f����Ȃ��悤�Ƀm�[�}���u�����h�ɖ߂�
	}

	void Logo::AnimationInit() {
		_pal = 0;
		_graphKey = TEAM_LOGO;	// �`�[�����S�ɐ؂�ւ�
	}

	void Logo::ChangeGraph() {
		_pal = 0;
		
		switch (_number) {

		}
	}
}
