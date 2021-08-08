#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(const Vector2& min, const Vector2& max, bool flg = false) : minV(min), maxV(max), _collisionFlg(flg){
#ifdef _DEBUG
	_drawFlg = true;
#endif
	_width = static_cast<int>(maxV.GetX() - minV.GetX());
	_height = static_cast<int>(maxV.GetX() - minV.GetX());
}

Collision::Collision(Vector2& pos, int width, int height, bool flg = false) {
	_width = width;
	_height = _height;
	_collisionFlg = false;
#ifdef _DEBUG
	_drawFlg = true;
#endif
	minV = { pos.GetX() - width, pos.GetY() - height };
	maxV = { pos.GetX() + width, pos.GetY() + height };
}

void Collision::Update(Vector2& pos, bool inv) {
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
		minV = { pos.GetX() - _width, pos.GetY() - _height };
		maxV = { pos.GetX() + _width, pos.GetY() + _height };
}

void Collision::Draw(int color) {
	auto minX = minV.IntX();
	auto minY = minV.IntY();
	auto maxX = maxV.IntX();
	auto maxY = maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
	bool flg = maxV.GetX() < collision.minV.GetX() ||
		collision.maxV.GetX() < minV.GetX() ||
		maxV.GetY() < collision.minV.GetY() ||
		collision.maxV.GetY() < minV.GetY();
	return !flg;
}

AABB::AABB(const Vector2& vmin, const Vector2& vmax, bool cflg) : Collision(vmin, vmax, cflg) {
	_width = 0;
	_height = 0;
	_fix = 0;
}

AABB::AABB(Vector2& pos, int width, int height, int fix, bool cflg) : Collision(pos, width, height, cflg) {
	_width = width;
	_height = height;
	_fix = fix;

	minV = { pos.GetX() - width, pos.GetY() - height + _fix};
	maxV = { pos.GetX() + width, pos.GetY() + height + _fix};
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