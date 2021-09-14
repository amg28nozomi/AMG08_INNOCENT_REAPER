#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

namespace hit {
	constexpr auto LEFT_HIT = 0;
	constexpr auto RIGHT_HIT = 1;
	constexpr auto NO_HIT = 1;
}

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
	_collisionFlg = flg;
#ifdef _DEBUG
	_drawFlg = true;
#endif
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

Collision::Collision() {
	_widthMin = 0;
	_widthMax = 0;
	_heightMin = 0;
	_heightMax = 0;
	_collisionFlg = false;
#ifdef _DEBUG
	_drawFlg = true;
#endif
	minV = {};
	maxV = {};
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

void Collision::DrawDBox(int color) {
	auto minX = minV.IntX();
	auto minY = minV.IntY();
	auto maxX = maxV.IntX();
	auto maxY = maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

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

bool Collision::HitUpDown(Collision col) {
	// 等しくない時は脱出
	if (_collisionFlg == true != col._collisionFlg == true) return false;
	// x軸は範囲内に収まっているか？
	if (minV.GetX() < col.maxV.GetX() && col.minV.GetX() < maxV.GetX()) {
		// 上から衝突したか？
		if (minV.GetY() < col.maxV.GetY() && col.minV.GetY() < maxV.GetY()) return true;
		// 下から衝突したか？
		if(maxV.GetY() < col.minV.GetY() && col.maxV.GetY() < minV.GetY()) return true;
	}
	return false;
}

double Collision::HitDirection(Collision col) {
	if (!_collisionFlg || !col._collisionFlg) return 0;
	// if (_collisionFlg == true != col._collisionFlg == true) return 0;
	// y座標は範囲内に収まっているか？
	if (minV.GetY() < col.maxV.GetY() && col.minV.GetY() < maxV.GetY()) {
		if (minV.GetX() < col.maxV.GetX() && col.maxV.GetX() < maxV.GetX()
			// maxVはチップminVよりも小さくてかつ、チップmaxV
			&& col.minV.GetX() < maxV.GetX() && col.maxV.GetX() < maxV.GetX()) {
				return (minV.GetX() - col.maxV.GetX()) * -1;
				//return (maxV.GetX() - col.minV.GetX()) * -1;
		}
	}
	return 0;
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