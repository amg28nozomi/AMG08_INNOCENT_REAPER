#pragma once
#include "Vector2.h"
#include <DxLib.h>

// 当たり判定の基底クラス
class Collision {
protected:
	Vector2 minV;
	Vector2 maxV;
	int _width;	// 幅
	int _height; // 高さ
#ifdef _DEBUG
	bool _drawFlg; // 描画するかどうか
#endif
	bool _collisionFlg; // 判定するかどうかのフラグ
public:
	Collision(Vector2& pos, int width, int height, bool flg = false);
	Collision(const Vector2& min, const Vector2& max, bool flg = false);
	~Collision() = default;

	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }

	virtual void Update(Vector2& pos, bool inv); // 更新・反転処理有
	void DrawBox(int color = GetColor(255, 255, 255)); // 描画(色指定なしの場合は白)
	bool HitCheck(Collision collision); // 当たり判定
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
private:
	int _fix;
public:
	AABB(const Vector2& vmin, const Vector2& vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, int fix =0 ,bool cflg = false);
	~AABB() = default;

	// 更新・反転処理有
	void Update(Vector2& pos, bool inv) override;
};
