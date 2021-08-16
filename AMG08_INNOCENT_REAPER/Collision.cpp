#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : minV(min), maxV(max),_collisionFlg(flg){
#ifdef _DEBUG
	_drawFlg = true;
#endif
	int width = static_cast<int>(maxV.GetX() - minV.GetX());
	int height = static_cast<int>(maxV.GetX() - minV.GetX());


	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;

	auto posx = max.GetX() - min.GetX() / 2;
	auto posy = max.GetY() - min.GetY() / 2;
	center = { posx, posy };
}

Collision::Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flg) {
	_widthMin = width1;
	_widthMax = width2;
	_heightMin = height1;
	_heightMax = height2;
	_collisionFlg = flg;

#ifdef _DEBUG
	_drawFlg = true;
#endif

	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision(Vector2& pos, int width, int height, bool flg) {
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	_collisionFlg = false;
#ifdef _DEBUG
	_drawFlg = true;
#endif
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

void Collision::Update(Vector2& pos, bool inv) {
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };

}

void Collision::Swap(Collision col){
	minV = col.GetMin();
	maxV = col.GetMax();
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };
}

//void Collision::DrawBox(int color) {
//	auto minX = minV.IntX();
//	auto minY = minV.IntY();
//	auto maxX = maxV.IntX();
//	auto maxY = maxV.IntY();
//
//	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
//}

bool Collision::HitCheck(Collision collision) {
	if (_collisionFlg == true && collision._collisionFlg == true) {
		bool flg = maxV.GetX() < collision.minV.GetX() ||
			collision.maxV.GetX() < minV.GetX() ||
			maxV.GetY() < collision.minV.GetY() ||
			collision.maxV.GetY() < minV.GetY();
		return !flg;
	}
	return false;	// 判定フラグがどちらか片方でもオフなら当たらない
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
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	if (inv) {
		minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin};
		maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };
}