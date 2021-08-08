#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

AABB::AABB(const Vector2& vmin, const Vector2& vmax, bool cflg) : minV(vmin), maxV(vmax), _drawFlg(true), _collisionFlg(cflg) {
}

AABB::AABB(Vector2& pos, int width, int height, int fix, bool cflg) {
	_width = width;
	_height = height;
	_fix = fix;

	minV = { pos.GetX() - width, pos.GetY() - height + _fix};
	maxV = { pos.GetX() + width, pos.GetY() + height + _fix};

	_drawFlg = true;
	_collisionFlg = cflg;
}

void AABB::Draw(int color) {
	auto minX = minV.IntX();
	auto minY = minV.IntY();
	auto maxX = maxV.IntX();
	auto maxY = maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

void AABB::SetVector(const Vector2& lhs, Vector2& rhs) {
	minV = lhs;
	maxV = rhs;
}

bool AABB::HitCheck(AABB collision) {
	bool flg = maxV.GetX() < collision.minV.GetX() ||
		collision.maxV.GetX() < minV.GetX() ||
		maxV.GetY() < collision.minV.GetY() ||
		collision.maxV.GetY() < minV.GetY();
	return !flg;
}

//void AABB::Update(Vector2& pos) {
//	auto minh = _height - _fix;
//	auto maxh = _height + _fix;
//
//	// 単位ベクトルを加算
//	minV = { pos.GetX() - _width, pos.GetY() - minh };
//	maxV = { pos.GetX() + _width, pos.GetY() + maxh };
//
//	/*minV.GetPX() += pos.GetX();
//	maxV.GetPX() += pos.GetX();
//	minV.GetPY() += pos.GetY();
//	maxV.GetPY() += pos.GetY();*/
//}

void AABB::Updata(Vector2& pos, bool inv) {
	auto minh = _height - _fix;
	auto maxh = _height + _fix;

	auto minw = _width - _fix;
	auto maxw = _width + _fix;
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	if (inv) {
		minV = { pos.GetX() - minw, pos.GetY() - minh};
		maxV = { pos.GetX() + _width, pos.GetY() + maxh };
		return;
	}
	minV = { pos.GetX() - _width , pos.GetY() - minh };
	maxV = { pos.GetX() + minw, pos.GetY() + maxh };
}

#ifdef _DEBUG
void AABB::DrawBox(){
	auto minX = minV.IntX();
	auto minY = minV.IntY();
	auto maxX = maxV.IntX();
	auto maxY = maxV.IntY();
	auto boxColor = GetColor(255, 255, 255);

	DxLib::DrawBox(minX, minY, maxX, maxY, boxColor, FALSE);
}
#endif