/*****************************************************************//**
 * \file   ForeGround.cpp
 * \brief  �O�i�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include "ForeGround.h"
#include "ImageValue.h"
#include "Game.h"
#include "StageTransition.h"
#include "Loads.h"
#include <DxLib.h>

namespace inr {
	// �R���X�g���N�^
	ForeGround::ForeGround(Game& game) : Image(game) {
		// �R���e�i�̎擾
		_ivalue = Loads::LoadForeGround1();
		_draw = false;
	}
	// �f�X�g���N�^
	ForeGround::~ForeGround() {
		_ivalue.clear();	// �R���e�i�̉��
	}
	// ������
	void ForeGround::Init() {
	}
	// �O�i�̐ݒ�
	void ForeGround::SetForeGround(std::vector<ImageValue> ive) {
		_ivalue = ive;	// �R���e�i�̓o�^
		return;
	}
	// �`��
	void ForeGround::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_graphKey, 0);	// �摜�̎擾
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);				// �`��
	}
	// �`����̐ݒ�
	bool ForeGround::SetKey(const std::string key) {
		auto ckey = CheckKey(key);		// �L�[�̓ǂݍ���
		if (ckey == -1) return false;	// �ǂݍ��ݎ��s
		switch (ckey) {
		case 0:	// �X�e�[�W0
			_draw = true;							// �ǉ��`�悠��
			_graphKey = _ivalue[ckey].GraphKey();	// �摜
			_pos = _ivalue[ckey].Position();		// ���W
			return true;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			_draw = false;							// �ǉ��`��Ȃ�
			_graphKey = _ivalue[ckey].GraphKey();	// �摜
			_pos = _ivalue[ckey].Position();		// ���W
			return true;
		default:
			return false;	// �Y���Ȃ�
		}
	}
	// �Y�����̎Z�o
	int ForeGround::CheckKey(const std::string key) {
		// �L�[�ɑΉ������Y������Ԃ�
		if (key == stage::STAGE_0) return 0;
		else if (key == stage::STAGE_T) return 1;
		else if (key == stage::STAGE_1) return 2;
		else if (key == stage::STAGE_2) return 3;
		else if (key == stage::STAGE_2_1) return 4;
		else if (key == stage::STAGE_2_2) return 5;
		else if (key == stage::STAGE_3) return 6;
		else return -1;		// �Y���Ȃ�
	}
}