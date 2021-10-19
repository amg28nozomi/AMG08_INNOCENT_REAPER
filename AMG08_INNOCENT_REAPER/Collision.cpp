/*****************************************************************//**
 * @file   Collision.cpp
 * @brief  �����s���E�{�b�N�X�̃X�[�p�[�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flag) : _minV(min), _maxV(max),_collisionFalg(flag){
#ifdef _DEBUG
	_drawFlag = true;		// �`��t���O���I��
#endif
	// �{�b�N�X�̉����Ɨ������Z�o
	int width = static_cast<int>(_maxV.GetX() - _minV.GetX());
	int height = static_cast<int>(_maxV.GetX() - _minV.GetX());
	// �e��C���l���
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	// ���S���W�ɓo�^
	auto posx = max.GetX() - min.GetX() / 2;
	auto posy = max.GetY() - min.GetY() / 2;
	_center = { posx, posy };
}

Collision::Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag) {
	// �e�평����
	_widthMin = width1;
	_widthMax = width2;
	_heightMin = height1;
	_heightMax = height2;
	_collisionFalg = flag;
#ifdef _DEBUG
	_drawFlag = true;
#endif
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision(Vector2& pos, int width, int height, bool flag) {
	// �e�평����
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	_collisionFalg = flag;
#ifdef _DEBUG
	_drawFlag = true;
#endif
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision() {
	// �e�평����
	_widthMin = 0;
	_widthMax = 0;
	_heightMin = 0;
	_heightMax = 0;
	_collisionFalg = false;
#ifdef _DEBUG
	_drawFlag = true;
#endif
	_minV = {};
	_maxV = {};
}

void Collision::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::Swap(Collision col){
	// �����蔻��̌�������яC��
	_minV = col.GetMin();
	_maxV = col.GetMax();
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::DrawColorBox(int color) {
	// �`����W�̎Z�o
	auto minX = _minV.IntX();
	auto minY = _minV.IntY();
	auto maxX = _maxV.IntX();
	auto maxY = _maxV.IntY();
	// �`�揈���Ăяo��
	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
	// ����t���O�̓I���ɂȂ��Ă��邩�H
	if (_collisionFalg == true && collision._collisionFalg == true) {
		// �ڐG���Ă��邩�H
		bool flag = _maxV.GetX() < collision._minV.GetX() ||
			collision._maxV.GetX() < _minV.GetX() ||
			_maxV.GetY() < collision._minV.GetY() ||
			collision._maxV.GetY() < _minV.GetY();
		return !flag;
	}
	return false;		// ����t���O���ǂ��炩�Е��ł��I�t�Ȃ瓖����Ȃ�
}