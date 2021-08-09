#pragma once
#include "Vector2.h"
#include <DxLib.h>

// �����蔻��̊��N���X
class Collision {
protected:
	Vector2 minV;
	Vector2 maxV;
	int _width;	// ��
	int _height; // ����
#ifdef _DEBUG
	bool _drawFlg; // �`�悷�邩�ǂ���
#endif
	bool _collisionFlg; // ���肷�邩�ǂ����̃t���O
public:
	Collision(Vector2& pos, int width, int height, bool flg = false);
	Collision(const Vector2& min, const Vector2& max, bool flg = false);
	~Collision() = default;

	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }

	virtual void Update(Vector2& pos, bool inv); // �X�V�E���]�����L
	void DrawBox(int color = GetColor(255, 255, 255)); // �`��(�F�w��Ȃ��̏ꍇ�͔�)
	bool HitCheck(Collision collision); // �����蔻��
	bool SideCheck(Collision collision);	// �ǂ��瑤�ŐڐG���Ă���̂��B
#ifdef _DEBUG
	inline bool GetDrawFlg() { return _drawFlg; }
	inline bool& GetbDrawFlg() { return _drawFlg; }
#endif
	inline bool GetCollisionFlg() { return _collisionFlg; }
	inline bool& GetCollisionFlgB() { return _collisionFlg; }

};

// ���]������ver
class AABB : public Collision {
private:
	int _fix;
public:
	AABB(const Vector2& vmin, const Vector2& vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, int fix =0 ,bool cflg = false);
	~AABB() = default;

	// �X�V�E���]�����L
	void Update(Vector2& pos, bool inv) override;
};
