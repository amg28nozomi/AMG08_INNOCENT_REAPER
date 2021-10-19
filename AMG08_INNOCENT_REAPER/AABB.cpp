/*****************************************************************//**
 * @file   AABB.cpp
 * @brief  反転処理がある当たり判定クラス（軸平行境界ボックスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "AABB.h"

AABB::AABB(Vector2 vmin, Vector2 vmax, bool cflag) : Collision(vmin, vmax, cflag) {
}

AABB::AABB(Vector2& pos, int width, int height, bool cflag) : Collision(pos, width, height, cflag) {
	// 各種初期化
	_widthMin = width;
	_widthMax = width;
	_heightMin = height;
	_heightMax = height;
	// 当たり判定の初期化
	_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
}

AABB::AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag) : Collision(vpos, width1, width2, height1, height2, cflag) {
}

void AABB::Update(Vector2& pos, bool inv) {
	// 向きに応じて当たり判定のx座標を変更する
	// 反転している場合
	if (inv) {
		_minV = { pos.GetX() - _widthMin, pos.GetY() - _heightMin };
		_maxV = { pos.GetX() + _widthMax, pos.GetY() + _heightMax };
		return;
	}
	// 反転処理無し
	_minV = { pos.GetX() - _widthMax , pos.GetY() - _heightMin };
	_maxV = { pos.GetX() + _widthMin, pos.GetY() + _heightMax };
	_center = { _maxV.GetX() - _minV.GetX(), _maxV.GetY() - _minV.GetY() };
}
