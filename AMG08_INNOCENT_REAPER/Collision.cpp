#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : minV(min), maxV(max),_collisionFlg(flg){
#ifdef _DEBUG
	_drawFlg = true;
#endif
	_width = static_cast<int>(maxV.GetX() - minV.GetX());
	_height = static_cast<int>(maxV.GetX() - minV.GetX());

	auto posx = max.GetX() - min.GetX() / 2;
	auto posy = max.GetY() - min.GetY() / 2;
	center = { posx, posy };
}

Collision::Collision(Vector2& pos, int width, int height, bool flg) {
	_width = width;
	_height = height;
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

void Collision::DrawBox(int color) {
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

bool Collision::SideCheck(Collision collision) {
	// 中心座標を取得
	auto y = collision.GetCenter().GetY();

	// 対象のy座標は判定内にあるかどうか？
	if (minV.GetY() <= y <= maxV.GetY()) {
		// xは中にあるかどうか？
		if (collision.minV.GetX() < maxV.GetX() && minV.GetX() < collision.maxV.GetX()) {
			return true;
		}
		// if (minV.GetX() <= x <= maxV.GetX()) {
		// }
	}
	return false;
}

AABB::AABB(Vector2 vmin, Vector2 vmax, bool cflg) : Collision(vmin, vmax, cflg) {
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

void AABB::Update(Vector2& pos, bool inv) {
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