#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : minV(min), maxV(max),_collisionFalg(flg){
#ifdef _DEBUG
	_drawFlag = true;
#endif
	// ボックスの横幅と立幅を算出
	int width = static_cast<int>(maxV.GetX() - minV.GetX());
	int height = static_cast<int>(maxV.GetX() - minV.GetX());


	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;

	// 中心座標に登録
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
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	center = { maxV.GetX() - minV.GetX(), maxV.GetY() - minV.GetY() };

}

void Collision::Swap(Collision col){
	// 当たり判定の交換
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
	// 判定フラグはオンになっているか？
	if (_collisionFalg == true && collision._collisionFalg == true) {
		// 接触しているか？
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
	if (_collisionFalg == true != col._collisionFalg == true) return false;
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
	if (!_collisionFalg || !col._collisionFalg) return 0;
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