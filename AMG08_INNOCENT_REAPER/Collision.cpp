#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : _minV(min), _maxV(max),_collisionFalg(flg){
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

Collision::Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flg) {
	_widthMin = width1;
	_widthMax = width2;
	_heightMin = height1;
	_heightMax = height2;
	_collisionFalg = flg;

#ifdef _DEBUG
	_drawFlag = true;
#endif

	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision(Vector2& pos, int width, int height, bool flg) {
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	_collisionFalg = flg;
#ifdef _DEBUG
	_drawFlag = true;
#endif
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision() {
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
	// ���]���Ă���ꍇ
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };

}

void Collision::Swap(Collision col){
	// �����蔻��̌���
	_minV = col.GetMin();
	_maxV = col.GetMax();
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::DrawDBox(int color) {
	// �`����W�Z�o
	auto minX = _minV.IntX();
	auto minY = _minV.IntY();
	auto maxX = _maxV.IntX();
	auto maxY = _maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
	// ����t���O�̓I���ɂȂ��Ă��邩�H
	if (_collisionFalg == true && collision._collisionFalg == true) {
		// �ڐG���Ă��邩�H
		bool flg = _maxV.GetX() < collision._minV.GetX() ||
			collision._maxV.GetX() < _minV.GetX() ||
			_maxV.GetY() < collision._minV.GetY() ||
			collision._maxV.GetY() < _minV.GetY();
		return !flg;
	}
	return false;	// ����t���O���ǂ��炩�Е��ł��I�t�Ȃ瓖����Ȃ�
}

bool Collision::HitUpDown(Collision col) {
	// �������Ȃ����͒E�o
	if (_collisionFalg == true != col._collisionFalg == true) return false;
	// x���͔͈͓��Ɏ��܂��Ă��邩�H
	if (_minV.GetX() < col._maxV.GetX() && col._minV.GetX() < _maxV.GetX()) {
		// �ォ��Փ˂������H
		if (_minV.GetY() < col._maxV.GetY() && col._minV.GetY() < _maxV.GetY()) return true;
		// ������Փ˂������H
		if(_maxV.GetY() < col._minV.GetY() && col._maxV.GetY() < _minV.GetY()) return true;
	}
	return false;
}

double Collision::HitDirection(Collision col) {
	if (!_collisionFalg || !col._collisionFalg) return 0;
	// y���W�͔͈͓��Ɏ��܂��Ă��邩�H
	if (_minV.GetY() < col._maxV.GetY() && col._minV.GetY() < _maxV.GetY()) {
		if (_minV.GetX() < col._maxV.GetX() && col._maxV.GetX() < _maxV.GetX()
			// maxV�̓`�b�vminV�����������Ă��A�`�b�vmaxV
			&& col._minV.GetX() < _maxV.GetX() && col._maxV.GetX() < _maxV.GetX()) {
				return (_minV.GetX() - col._maxV.GetX()) * -1;
				//return (maxV.GetX() - col.minV.GetX()) * -1;
		}
	}
	return 0;
}


bool Collision::SideCheck(Collision collision) {
	// ���S���W���擾
	auto y = collision.GetCenter().GetY();

	// �Ώۂ�y���W�͔�����ɂ��邩�ǂ����H
	if (_minV.GetY() <= y <= _maxV.GetY()) {
		// x�͒��ɂ��邩�ǂ����H
		if (collision._minV.GetX() < _maxV.GetX() && _minV.GetX() < collision._maxV.GetX()) {
			return true;
		}
	}
	return false;
}

AABB::AABB(Vector2 vmin, Vector2 vmax, bool cflg) : Collision(vmin, vmax, cflg) {
}

AABB::AABB(Vector2& pos, int width, int height, bool cflg) : Collision(pos, width, height, cflg) {
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;

	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

AABB::AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflg) : Collision(vpos, width1, width2, height1, height2, cflg) {

}

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