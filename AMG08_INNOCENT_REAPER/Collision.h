/*****************************************************************//**
 * \file   Collision.h
 * \brief  当たり判定クラス
 *		   軸平行境界ボックスクラス（当たり判定クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include "Vector2.h"

// 当たり判定の基底クラス
class Collision {
protected:
	Vector2 _minV;			// 2Dベクトル(Min)
	Vector2 _maxV;			// 2Dベクトル(Max)
	Vector2 _center;		// 中心座標
	int _widthMin;			// 幅min
	int _widthMax;			// 幅max
	int _heightMin;			// 高さmin
	int _heightMax;			// 高さmax
#ifdef _DEBUG
	bool _drawFlag;			// 描画するかどうか
#endif
	bool _collisionFalg;	// 判定するかどうかのフラグ
public:
	// コンストラクタ(引数1:中心座標　引数2:横幅(min)　引数3:横幅(max)　引数4:立幅(min)　引数5:立幅(max)　引数6:判定フラグ)
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag = false);
	// コンストラクタ(引数1:中心座標　引数2:横幅　引数3:縦幅　引数4:判定フラグ)
	Collision(Vector2& pos, int width, int height, bool flag = false);
	// コンストラクタ(引数1:ベクトル2(min)　引数2:ベクトル2(max)　引数3:判定フラグ)
	Collision(Vector2& min, Vector2& max, bool flag = false);
	/**
	 * @brief	コンストラクタ
	 */
	Collision();
	// デストラクタ
	~Collision() = default;
	// 2Dベクトル(Min)の取得
	inline Vector2 GetMin() { return _minV; }
	// 2Dベクトル(Max)の取得
	inline Vector2 GetMax() { return _maxV; }
	// 中心座標の取得
	inline Vector2 GetCenter() { return _center; }
	// 横幅(min)の取得
	inline int GetWidthMin() { return _widthMin; }
	// 横幅(max)の取得
	inline int GetWidthMax() { return _widthMax; }
	// 縦幅(min)の取得
	inline int GetHeightMin() { return _heightMin; }
	// 縦幅(max)の取得
	inline int GetHeightMax() { return _heightMax; }
	// 横幅の取得
	inline int WidhtSize() { return _widthMin + _widthMax; }
	// 縦幅の取得
	inline int HeightSize() { return _heightMin + _heightMax; }
	// 当たり判定の更新
	virtual void Update(Vector2& pos, bool inv);
	// 引数として渡された当たり判定と自身の当たり判定を切り替える
	virtual void Swap(Collision col);
	// ボックスの描画(引数による色指定なしの場合は白描画)
	void DrawColorBox(int color = GetColor(255, 255, 255));
	// 当たり判定同士の衝突判定(引数:衝突判定を行う対象)
	bool HitCheck(Collision collision);
#ifdef _DEBUG
	// 描画フラグの取得
	inline bool GetDrawFlag() { return _drawFlag; }
	// 描画フラグの参照
	inline bool& SetDrawFlag() { return _drawFlag; }
#endif
	// 判定フラグの取得
	inline bool GetCollisionFlag() { return _collisionFalg; }
	// 判定フラグの参照
	inline bool& SetCollisionFlag() { return _collisionFalg; }
};
// 反転処理が有る当たり判定クラス
class AABB : public Collision {
public:
	// コンストラクタ(引数1:移動ベクトル(min)　引数2:移動ベクトル(max)　引数3:判定フラグ)
	AABB(Vector2 vmin, Vector2 vmax, bool cflag = false);
	// コンストラクタ(引数1:中心座標　引数2:横幅　引数3:縦幅　引数4:判定フラグ)
	AABB(Vector2& pos, int width, int height, bool cflag = false);
	// コンストラクタ(引数1:中心座標　引数2:横幅(min)　引数3:横幅(max)　引数4:立幅(min)　引数5:立幅(max)　引数6:判定フラグ)
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag = false);
	// デストラクタ
	~AABB() = default;
	// 当たり判定の更新(反転処理有り)
	void Update(Vector2& pos, bool inv) override;
};
