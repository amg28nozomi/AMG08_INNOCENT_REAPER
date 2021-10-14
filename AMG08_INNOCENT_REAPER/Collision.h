/*****************************************************************//**
 * @file   Collision.h
 * @brief  当たり判定クラス
 *		   軸平行境界ボックスクラス（当たり判定クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include "Vector2.h"

/**   当たり判定の基底クラス	*/
class Collision {
protected:
	Vector2 _minV;				//!< 2Dベクトル(Min)
	Vector2 _maxV;				//!< 2Dベクトル(Max)
	Vector2 _center;			//!< 中心座標
	int _widthMin;				//!< 幅min
	int _widthMax;				//!< 幅max
	int _heightMin;				//!< 高さmin
	int _heightMax;				//!< 高さmax
	bool _collisionFalg;	//!< 判定するかどうかのフラグ
#ifdef _DEBUG
	bool _drawFlag;				// 描画するかどうか
#endif
public:
	/**
	 * @brief					コンストラクタ
	 * @param pos			中心座標
	 * @param width1	横幅(min)
	 * @param width2	横幅(max)
	 * @param height1	縦幅(min)
	 * @param height2	縦幅(max)
	 * @param flag		当たり判定を行うかのフラグ
	 */
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag = false);
	/**
	 * @brief					コンストラクタ
	 * @param pos			中心座標
	 * @param width		横幅
	 * @param height	縦幅
	 * @param flag		当たり判定を行うかのフラグ
	 */
	Collision(Vector2& pos, int width, int height, bool flag = false);
	/**
	 * @brief				コンストラクタ
	 * @param	min		座標ベクトル2(min)
	 * @param	max		座標ベクトル2(max)
	 * @param	flag	当たり判定を行うかのフラグ
	 */
	Collision(Vector2& min, Vector2& max, bool flag = false);
	/**
	 * @brief	コンストラクタ
	 */
	Collision();
	/**
	 * @brief	デストラクタ
	 */
	~Collision() = default;
	/**
	 * @brief		2Dベクトル(min)の取得
	 * @return	2Dベクトル(min)を返す
	 */
	inline Vector2 GetMin() { return _minV; }
	/**
	 * @brief		2Dベクトル(max)の取得
	 * @return	2Dベクトル(max)を返す
	 */
	inline Vector2 GetMax() { return _maxV; }
	/**
	 * @brief		中心座標の取得
	 * @return	中心座標を返す
	 */
	inline Vector2 GetCenter() { return _center; }
	/**
	 * @brief		横幅(min)の取得
	 * @return	横幅(min)を返す
	 */
	inline int GetWidthMin() { return _widthMin; }
	/**
	 * @brief		横幅(max)の取得 
	 * @return	横幅(max)を返す
	 */
	inline int GetWidthMax() { return _widthMax; }
	/**
	 * @brief		縦幅(min)の取得
	 * @return	縦幅(min)を返す
	 */
	inline int GetHeightMin() { return _heightMin; }
	/**
	 * @brief		縦幅(max)の取得
	 * @return	縦幅(max)を返す
	 */
	inline int GetHeightMax() { return _heightMax; }
	/**
	 * @brief		横幅の取得
	 * @return	横幅を返す
	 */
	inline int WidhtSize() { return _widthMin + _widthMax; }
	/**
	 * @brief		縦幅の取得
	 * @return	縦幅を返す
	 */
	inline int HeightSize() { return _heightMin + _heightMax; }
	/**
	 * @brief			当たり判定の更新
	 * @param pos	現在の移動ベクトル
	 * @param inv	現在の向き
	 */
	virtual void Update(Vector2& pos, bool inv);
	/**
	 * @brief			一時的に当たり判定を代入する
	 * @param col	代入する当たり判定
	 */
	virtual void Swap(Collision col);
	/**
	 * @brief				ボックスの描画
	 * @param color	描画ボックスの色(引数による色指定なしの場合は白)
	 */
	void DrawColorBox(int color = GetColor(255, 255, 255));
	/**
	 * @brief						当たり判定同士の衝突判定
	 * @param collision	衝突判定を行う対象
	 * @return					衝突した場合はtrueを返す
	 * @return					衝突していない場合はfalseを返す
	 */
	bool HitCheck(Collision collision);
#ifdef _DEBUG
	/**
	 * @brief		描画フラグの取得
	 * @return	描画フラグを返す
	 */
	inline bool GetDrawFlag() { return _drawFlag; }
	/**
	 * @brief		描画フラグの参照を取得
	 * @return	描画フラグの参照を返す
	 */
	inline bool& SetDrawFlag() { return _drawFlag; }
#endif
	/**
	 * @brief		判定フラグの取得
	 * @return	判定フラグを返す
	 */
	inline bool GetCollisionFlag() { return _collisionFalg; }
	/**
	 * @brief		判定フラグの参照を取得
	 * @return	判定フラグの参照を返す
	 */
	inline bool& SetCollisionFlag() { return _collisionFalg; }
};
/** 反転処理が有る当たり判定クラス */
class AABB : public Collision {
public:
	/**
	 * @brief				コンストラクタ
	 * @param	min		座標ベクトル2(min)
	 * @param	max		座標ベクトル2(max)
	 * @param	flag	当たり判定を行うかのフラグ
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
	 * @brief	デストラクタ
	 */
	~AABB() = default;
	// 当たり判定の更新(反転処理有り)
	/**
	 * @brief			当たり判定の更新(向きに応じた反転処理有り)
	 * \param pos	現在の座標ベクトル
	 * \param inv	現在の向き
	 */
	void Update(Vector2& pos, bool inv) override;
};
