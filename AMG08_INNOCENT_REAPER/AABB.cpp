/*****************************************************************//**
 * @file   AABB.cpp
 * @brief  ���]���������铖���蔻��N���X�i�����s���E�{�b�N�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "AABB.h"

AABB::AABB(Vector2 vmin, Vector2 vmax, bool cflag) : Collision(vmin, vmax, cflag) {
}

AABB::AABB(Vector2& pos, int width, int height, bool cflag) : Collision(pos, width, height, cflag) {
	// �e�평����
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	// �����蔻��̏�����
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

AABB::AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag) : Collision(vpos, width1, width2, height1, height2, cflag) {
}

void AABB::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	// ���]���Ă���ꍇ
	if (inv) {
		_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
		_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	// ���]��������
	_minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}
