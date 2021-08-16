#pragma once
#include <DxLib.h>
#include "Vector2.h"

// 当たり判定の基底クラス
class Collision {
protected:
	Vector2 minV;
	Vector2 maxV;
	Vector2 center;	// 中心座標

	int _widthMin;	// 幅min
	int _widthMax;	// 幅max
	int _heightMin; // 高さmin
	int _heightMax;	// 高さmax
#ifdef _DEBUG
	bool _drawFlg; // 描画するかどうか
#endif
	bool _collisionFlg; // 判定するかどうかのフラグ
public:
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flg = false);
	Collision(Vector2& pos, int width, int height, bool flg = false);
	Collision(Vector2& min, Vector2& max, bool flg = false);
	~Collision() = default;

	// 参照ゲッター（Reference）
	/*inline Vector2& RefMin() { return minV; }
	inline Vector2& RefMax() {return maxV;}*/

	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }
	inline Vector2 GetCenter() { return center; }
	/*inline const int GetWidth() { return _width / 2; }
	inline const int GetHeight() { return _height / 2; }*/

	virtual void Update(Vector2& pos, bool inv); // 更新・反転処理有
	virtual void Swap(Collision col);	// 一時的に当たり判定を切り替える
	//void DrawBox(int color = GetColor(255, 255, 255)); // 描画(色指定なしの場合は白)
	bool HitCheck(Collision collision); // 全方向の当たり判定
	bool HitUpDown(Collision col);	// 上下の当たり判定
	double HitDirection(Collision col);	// 左右の当たり判定
	bool SideCheck(Collision collision);	// どちら側で接触しているのか。
#ifdef _DEBUG
	inline bool GetDrawFlg() { return _drawFlg; }
	inline bool& GetbDrawFlg() { return _drawFlg; }
#endif
	inline bool GetCollisionFlg() { return _collisionFlg; }
	inline bool& GetCollisionFlgB() { return _collisionFlg; }

};

// 反転があるver
class AABB : public Collision {
public:
	AABB(Vector2 vmin, Vector2 vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, bool cflg = false);
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflg = false);
	~AABB() = default;

	// 更新・反転処理有
	void Update(Vector2& pos, bool inv) override;
};
