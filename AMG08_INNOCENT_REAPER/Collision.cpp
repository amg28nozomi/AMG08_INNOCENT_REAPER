#include "Collision.h"
#include "Vector2.h"
#include <DxLib.h>

Collision::Collision(Vector2& min, Vector2& max, bool flg) : _minV(min), _maxV(max),_collisionFalg(flg){
#ifdef _DEBUG
	_drawFlag = true;	// 描画フラグをオン
#endif
	// ボックスの横幅と立幅を算出
	int width = static_cast<int>(_maxV.GetX() - _minV.GetX());
	int height = static_cast<int>(_maxV.GetX() - _minV.GetX());

	// 各種修正値代入
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;

	// 中心座標に登録
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
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };

}

void Collision::Swap(Collision col){
	// 当たり判定の交換
	_minV = col.GetMin();
	_maxV = col.GetMax();
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}

void Collision::DrawDBox(int color) {
	// 描画座標算出
	auto minX = _minV.IntX();
	auto minY = _minV.IntY();
	auto maxX = _maxV.IntX();
	auto maxY = _maxV.IntY();

	DxLib::DrawBox(minX, minY, maxX, maxY, color, FALSE);
}

bool Collision::HitCheck(Collision collision) {
	// 判定フラグはオンになっているか？
	if (_collisionFalg == true && collision._collisionFalg == true) {
		// 接触しているか？
		bool flg = _maxV.GetX() < collision._minV.GetX() ||
			collision._maxV.GetX() < _minV.GetX() ||
			_maxV.GetY() < collision._minV.GetY() ||
			collision._maxV.GetY() < _minV.GetY();
		return !flg;
	}
	return false;	// 判定フラグがどちらか片方でもオフなら当たらない
}

bool Collision::HitUpDown(Collision col) {
	// 等しくない時は脱出
	if (_collisionFalg == true != col._collisionFalg == true) return false;
	// x軸は範囲内に収まっているか？
	if (_minV.GetX() < col._maxV.GetX() && col._minV.GetX() < _maxV.GetX()) {
		// 上から衝突したか？
		if (_minV.GetY() < col._maxV.GetY() && col._minV.GetY() < _maxV.GetY()) return true;
		// 下から衝突したか？
		if(_maxV.GetY() < col._minV.GetY() && col._maxV.GetY() < _minV.GetY()) return true;
	}
	return false;
}

double Collision::HitDirection(Collision col) {
	if (!_collisionFalg || !col._collisionFalg) return 0;
	// y座標は範囲内に収まっているか？
	if (_minV.GetY() < col._maxV.GetY() && col._minV.GetY() < _maxV.GetY()) {
		if (_minV.GetX() < col._maxV.GetX() && col._maxV.GetX() < _maxV.GetX()
			// maxVはチップminVよりも小さくてかつ、チップmaxV
			&& col._minV.GetX() < _maxV.GetX() && col._maxV.GetX() < _maxV.GetX()) {
				return (_minV.GetX() - col._maxV.GetX()) * -1;
				//return (maxV.GetX() - col.minV.GetX()) * -1;
		}
	}
	return 0;
}


bool Collision::SideCheck(Collision collision) {
	// 中心座標を取得
	auto y = collision.GetCenter().GetY();

	// 対象のy座標は判定内にあるかどうか？
	if (_minV.GetY() <= y <= _maxV.GetY()) {
		// xは中にあるかどうか？
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
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	if (inv) {
		_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin};
		_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	// 反転処理無し
	_minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}