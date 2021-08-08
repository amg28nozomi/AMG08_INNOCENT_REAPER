#pragma once
#include "Vector2.h"
#include <DxLib.h>

class AABB {
private:
	Vector2 minV;
	Vector2 maxV;
	int _width;	// 幅
	int _height; // 高さ
	int _fix;
	bool _drawFlg; // 描画するかどうか
	bool _collisionFlg; // 判定するかどうかのフラグ
public:
	AABB(const Vector2& vmin, const Vector2& vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, int fix =0 ,bool cflg = false);
	~AABB() = default;

	void Draw(int color = GetColor(255, 255, 255)); // 描画(色指定なしの場合は白)
	void SetVector(const Vector2& lhs, Vector2& rhs);
	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }
	// 描画するかどうかの判定
	inline bool GetDrawFlg() { return _drawFlg; }
	// 参照渡し
	inline bool& GetbDrawFlg() { return _drawFlg; }
	// フラグ取得用
	inline bool GetCollisionFlg() { return _collisionFlg; }
	// 参照渡し
	inline bool& GetCollisionFlgB() { return _collisionFlg; }

	// 当たり判定
	bool HitCheck(AABB collision);
	// 更新・反転処理無
	// void Update(Vector2& pos);
	// 更新・反転処理有
	void Updata(Vector2& pos, bool inv);
#ifdef _DEBUG
	// デバッグ用:AABBの描画
	void DrawBox();
#endif
};
