#pragma once
#include <DxLib.h>
#include "Vector2.h"

// 当たり判定の基底クラス
class Collision {
protected:
	Vector2 _minV;	// 2Dベクトル(Min)
	Vector2 _maxV;	// 2Dベクトル(MAx)
	Vector2 _center;	// 中心座標

	int _widthMin;	// 幅min
	int _widthMax;	// 幅max
	int _heightMin; // 高さmin
	int _heightMax;	// 高さmax
#ifdef _DEBUG
	bool _drawFlag; // 描画するかどうか
#endif
	bool _collisionFalg; // 判定するかどうかのフラグ
public:
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flg = false);
	Collision(Vector2& pos, int width, int height, bool flg = false);
	Collision(Vector2& min, Vector2& max, bool flg = false);
	Collision();
	~Collision() = default;

	// 2Dベクトル(Min)の取得
	inline Vector2 GetMin() { return _minV; }
	// 2Dベクトル(Max)の取得
	inline Vector2 GetMax() { return _maxV; }
	// 中心座標の取得
	inline Vector2 GetCenter() { return _center; }

	// 各種補正値の取得
	inline int GetWidthMin() { return _widthMin; }
	inline int GetWidthMax() { return _widthMax; }
	inline int GetHeightMin() { return _heightMin; }
	inline int GetHeightMax() { return _heightMax; }
	inline int WidhtSize() { return _widthMin + _widthMax; }
	inline int HeightSize() { return _heightMin + _heightMax; }

	// 更新・反転処理有
	virtual void Update(Vector2& pos, bool inv);
	// 一時的に当たり判定を切り替える
	virtual void Swap(Collision col);
	// ボックスの描画(色指定なしの場合は白描画)
	void DrawDBox(int color = GetColor(255, 255, 255));
	// 全方向の当たり判定
	bool HitCheck(Collision collision);
	// 上下の当たり判定
	bool HitUpDown(Collision col);
	// 左右の当たり判定
	double HitDirection(Collision col);
	// どちら側で接触しているのか。
	bool SideCheck(Collision collision);
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

// 当たり判定(反転処理有り)
class AABB : public Collision {
public:
	AABB(Vector2 vmin, Vector2 vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, bool cflg = false);
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflg = false);
	~AABB() = default;

	// 更新・反転処理有
	void Update(Vector2& pos, bool inv) override;
};
