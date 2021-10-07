#pragma once
#include <DxLib.h>
#include "Vector2.h"

// �����蔻��̊��N���X
class Collision {
protected:
	Vector2 _minV;	// 2D�x�N�g��(Min)
	Vector2 _maxV;	// 2D�x�N�g��(Max)
	Vector2 _center;	// ���S���W
	int _widthMin;	// ��min
	int _widthMax;	// ��max
	int _heightMin; // ����min
	int _heightMax;	// ����max
#ifdef _DEBUG
	bool _drawFlag; // �`�悷�邩�ǂ���
#endif
	bool _collisionFalg; // ���肷�邩�ǂ����̃t���O
public:
	// ����1:���S���W�@����2:����(min)�@����3:����(max)�@����4:����(min)�@����5:����(max)�@����6:���]�t���O
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag = false);
	Collision(Vector2& pos, int width, int height, bool flag = false);
	Collision(Vector2& min, Vector2& max, bool flag = false);
	Collision();
	~Collision() = default;
	// 2D�x�N�g��(Min)�̎擾
	inline Vector2 GetMin() { return _minV; }
	// 2D�x�N�g��(Max)�̎擾
	inline Vector2 GetMax() { return _maxV; }
	// ���S���W�̎擾
	inline Vector2 GetCenter() { return _center; }
	// �e��␳�l�̎擾
	inline int GetWidthMin() { return _widthMin; }
	inline int GetWidthMax() { return _widthMax; }
	inline int GetHeightMin() { return _heightMin; }
	inline int GetHeightMax() { return _heightMax; }
	inline int WidhtSize() { return _widthMin + _widthMax; }
	inline int HeightSize() { return _heightMin + _heightMax; }
	// �����蔻��̍X�V
	virtual void Update(Vector2& pos, bool inv);
	// �����Ƃ��ēn���ꂽ�����蔻��Ǝ��g�̓����蔻���؂�ւ���
	virtual void Swap(Collision col);
	// �{�b�N�X�̕`��(�����ɂ��F�w��Ȃ��̏ꍇ�͔��`��)
	void DrawColorBox(int color = GetColor(255, 255, 255));
	// �����蔻�蓯�m�̏Փ˔���(����:�Փ˔�����s���Ώ�)
	bool HitCheck(Collision collision);
#ifdef _DEBUG
	// �`��t���O�̎擾
	inline bool GetDrawFlag() { return _drawFlag; }
	// �`��t���O�̎Q��
	inline bool& SetDrawFlag() { return _drawFlag; }
#endif
	// ����t���O�̎擾
	inline bool GetCollisionFlag() { return _collisionFalg; }
	// ����t���O�̎Q��
	inline bool& SetCollisionFlag() { return _collisionFalg; }
};
// ���]�������L�铖���蔻��N���X
class AABB : public Collision {
public:
	AABB(Vector2 vmin, Vector2 vmax, bool cflag = false);
	AABB(Vector2& vmin, int width, int height, bool cflag = false);
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag = false);
	~AABB() = default;
	// �����蔻��̍X�V(���]�����L��)
	void Update(Vector2& pos, bool inv) override;
};
