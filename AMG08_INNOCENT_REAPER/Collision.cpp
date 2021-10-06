#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : minV(min), maxV(max),_collisionFalg(flg){
#ifdef _DEBUG
	_drawFlag = true;
#endif
	// �{�b�N�X�̉����Ɨ������Z�o
	int width = static_cast<int>(maxV.GetX() - minV.GetX());
	int height = static_cast<int>(maxV.GetX() - minV.GetX());


	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;

	// ���S���W�ɓo�^
	auto posx = max.GetX() - min.GetX() / 2;
	auto posy = max.GetY() - min.GetY() / 2;
	center = { posx, posy };
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

	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
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
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
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
	minV = {};
	maxV = {};
}

void Collision::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	// ���]���Ă���ꍇ
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };

}

void Collision::Swap(Collision col){
	// �����蔻��̌���
	minV = col.GetMin();
	maxV = col.GetMax();
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };
}

void Collision::DrawDBox(int color) {
	auto minX = minV.IntX();
	auto minY = minV.IntY();
	auto maxX = maxV.IntX();
	auto maxY = maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
	// ����t���O�̓I���ɂȂ��Ă��邩�H
	if (_collisionFalg == true && collision._collisionFalg == true) {
		// �ڐG���Ă��邩�H
		bool flg = maxV.GetX() < collision.minV.GetX() ||
			collision.maxV.GetX() < minV.GetX() ||
			maxV.GetY() < collision.minV.GetY() ||
			collision.maxV.GetY() < minV.GetY();
		return !flg;
	}
	return false;	// ����t���O���ǂ��炩�Е��ł��I�t�Ȃ瓖����Ȃ�
}

bool Collision::HitUpDown(Collision col) {
	// �������Ȃ����͒E�o
	if (_collisionFalg == true != col._collisionFalg == true) return false;
	// x���͔͈͓��Ɏ��܂��Ă��邩�H
	if (minV.GetX() < col.maxV.GetX() && col.minV.GetX() < maxV.GetX()) {
		// �ォ��Փ˂������H
		if (minV.GetY() < col.maxV.GetY() && col.minV.GetY() < maxV.GetY()) return true;
		// ������Փ˂������H
		if(maxV.GetY() < col.minV.GetY() && col.maxV.GetY() < minV.GetY()) return true;
	}
	return false;
}

double Collision::HitDirection(Collision col) {
	if (!_collisionFalg || !col._collisionFalg) return 0;
	// y���W�͔͈͓��Ɏ��܂��Ă��邩�H
	if (minV.GetY() < col.maxV.GetY() && col.minV.GetY() < maxV.GetY()) {
		if (minV.GetX() < col.maxV.GetX() && col.maxV.GetX() < maxV.GetX()
			// maxV�̓`�b�vminV�����������Ă��A�`�b�vmaxV
			&& col.minV.GetX() < maxV.GetX() && col.maxV.GetX() < maxV.GetX()) {
				return (minV.GetX() - col.maxV.GetX()) * -1;
				//return (maxV.GetX() - col.minV.GetX()) * -1;
		}
	}
	return 0;
}


bool Collision::SideCheck(Collision collision) {
	// ���S���W���擾
	auto y = collision.GetCenter().GetY();

	// �Ώۂ�y���W�͔�����ɂ��邩�ǂ����H
	if (minV.GetY() <= y <= maxV.GetY()) {
		// x�͒��ɂ��邩�ǂ����H
		if (collision.minV.GetX() < maxV.GetX() && minV.GetX() < collision.maxV.GetX()) {
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

	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

AABB::AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflg) : Collision(vpos, width1, width2, height1, height2, cflg) {

}

void AABB::Update(Vector2& pos, bool inv) {
	// �����ɉ����ē����蔻���x���W��ύX����
	// ���]���Ă���ꍇ
	if (inv) {
		minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin};
		maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };
}