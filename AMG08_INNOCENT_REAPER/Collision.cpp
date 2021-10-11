/*****************************************************************//**
 * \file   Collision.cpp
 * \brief  �����蔻��N���X
 *		   �����s���E�{�b�N�X�N���X�i�����蔻��N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>
// �R���X�g���N�^
Collision::Collision(Vector2& min, Vector2& max, bool flag) : _minV(min), _maxV(max),_collisionFalg(flag){
#ifdef _DEBUG
	_drawFlag = true;	// �`��t���O���I��
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
// �R���X�g���N�^
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
// �R���X�g���N�^
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
// �R���X�g���N�^
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
// �X�V
void Collision::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };

}
// �����蔻��̌���
void Collision::Swap(Collision col){
	// �����蔻��̌�������яC��
	_minV = col.GetMin();
	_maxV = col.GetMax();
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}
// �{�b�N�X�̕`��
void Collision::DrawColorBox(int color) {
	// �`����W�̎Z�o
	auto minX = _minV.IntX();
	auto minY = _minV.IntY();
	auto maxX = _maxV.IntX();
	auto maxY = _maxV.IntY();
	// �`�揈���Ăяo��
	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}
// �{�b�N�X���m�̏Փ˔���
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
	return false;	// ����t���O���ǂ��炩�Е��ł��I�t�Ȃ瓖����Ȃ�
}
// �R���X�g���N�^
AABB::AABB(Vector2 vmin, Vector2 vmax, bool cflag) : Collision(vmin, vmax, cflag) {
}
// �R���X�g���N�^
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
// �R���X�g���N�^
AABB::AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag) : Collision(vpos, width1, width2, height1, height2, cflag) {
}
// �����蔻��̍X�V
void AABB::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	// ���]���Ă���ꍇ
	if (inv) {
		_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin};
		_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	// ���]��������
	_minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}