#pragma once
#include <DxLib.h>
#include "Vector2.h"

// �����蔻��̊��N���X
class Collision {
protected:
	Vector2 minV;
	Vector2 maxV;
	Vector2 center;	// ���S���W

	int _widthMin;	// ��min
	int _widthMax;	// ��max
	int _heightMin; // ����min
	int _heightMax;	// ����max
#ifdef _DEBUG
	bool _drawFlg; // �`�悷�邩�ǂ���
#endif
	bool _collisionFlg; // ���肷�邩�ǂ����̃t���O
public:
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flg = false);
	Collision(Vector2& pos, int width, int height, bool flg = false);
	Collision(Vector2& min, Vector2& max, bool flg = false);
	~Collision() = default;

	// �Q�ƃQ�b�^�[�iReference�j
	/*inline Vector2& RefMin() { return minV; }
	inline Vector2& RefMax() {return maxV;}*/

	inline Vector2 GetMin() { return minV; }
	inline Vector2 GetMax() { return maxV; }
	inline Vector2 GetCenter() { return center; }
	/*inline const int GetWidth() { return _width / 2; }
	inline const int GetHeight() { return _height / 2; }*/

	virtual void Update(Vector2& pos, bool inv); // �X�V�E���]�����L
	virtual void Swap(Collision col);	// �ꎞ�I�ɓ����蔻���؂�ւ���
	//void DrawBox(int color = GetColor(255, 255, 255)); // �`��(�F�w��Ȃ��̏ꍇ�͔�)
	bool HitCheck(Collision collision); // �S�����̓����蔻��
	bool HitUpDown(Collision col);	// �㉺�̓����蔻��
	double HitDirection(Collision col);	// ���E�̓����蔻��
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
public:
	AABB(Vector2 vmin, Vector2 vmax, bool cflg = false);
	AABB(Vector2& vmin, int width, int height, bool cflg = false);
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflg = false);
	~AABB() = default;

	// �X�V�E���]�����L
	void Update(Vector2& pos, bool inv) override;
};
