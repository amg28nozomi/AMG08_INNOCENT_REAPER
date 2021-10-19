/*****************************************************************//**
 * @file   AABB.h
 * @brief  反転処理がある当たり判定クラス（軸平行境界ボックスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Collision.h"

/** 反転処理が有る当たり判定クラス */
class AABB : public Collision {
public:
	/**
	 * @brief					コンストラクタ
	 * @param	min			座標ベクトル2(min)
	 * @param	max			座標ベクトル2(max)
	 * @param	flag		当たり判定を行うかのフラグ
	 */
	AABB(Vector2 vmin, Vector2 vmax, bool cflag = false);
	/**
	 * @brief					コンストラクタ
	 * @param pos			中心座標
	 * @param width		横幅
	 * @param height	縦幅
	 * @param flag		当たり判定を行うかのフラグ
	 */
	AABB(Vector2& pos, int width, int height, bool cflag = false);
	/**
	 * @brief					コンストラクタ
	 * @param pos			中心座標
	 * @param width1	横幅(min)
	 * @param width2	横幅(max)
	 * @param height1	縦幅(min)
	 * @param height2	縦幅(max)
	 * @param flag		当たり判定を行うかのフラグ
	 */
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag = false);
	/**
	 * @brief					デストラクタ
	 */
	~AABB() = default;
	/**
	 * @brief					当たり判定の更新(向きに応じた反転処理有り)
	 * @param pos			現在の座標ベクトル
	 * @param inv			現在の向き
	 */
	void Update(Vector2& pos, bool inv) override;
};
