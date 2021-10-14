/*****************************************************************//**
 * \file   Collision.h
 * \brief  �����蔻��N���X
 *		   �����s���E�{�b�N�X�N���X�i�����蔻��N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include "Vector2.h"

// �����蔻��̊��N���X
class Collision {
protected:
	Vector2 _minV;			// 2D�x�N�g��(Min)
	Vector2 _maxV;			// 2D�x�N�g��(Max)
	Vector2 _center;		// ���S���W
	int _widthMin;			// ��min
	int _widthMax;			// ��max
	int _heightMin;			// ����min
	int _heightMax;			// ����max
#ifdef _DEBUG
	bool _drawFlag;			// �`�悷�邩�ǂ���
#endif
	bool _collisionFalg;	// ���肷�邩�ǂ����̃t���O
public:
	// �R���X�g���N�^(����1:���S���W�@����2:����(min)�@����3:����(max)�@����4:����(min)�@����5:����(max)�@����6:����t���O)
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag = false);
	// �R���X�g���N�^(����1:���S���W�@����2:�����@����3:�c���@����4:����t���O)
	Collision(Vector2& pos, int width, int height, bool flag = false);
	// �R���X�g���N�^(����1:�x�N�g��2(min)�@����2:�x�N�g��2(max)�@����3:����t���O)
	Collision(Vector2& min, Vector2& max, bool flag = false);
	/**
	 * @brief	�R���X�g���N�^
	 */
	Collision();
	// �f�X�g���N�^
	~Collision() = default;
	// 2D�x�N�g��(Min)�̎擾
	inline Vector2 GetMin() { return _minV; }
	// 2D�x�N�g��(Max)�̎擾
	inline Vector2 GetMax() { return _maxV; }
	// ���S���W�̎擾
	inline Vector2 GetCenter() { return _center; }
	// ����(min)�̎擾
	inline int GetWidthMin() { return _widthMin; }
	// ����(max)�̎擾
	inline int GetWidthMax() { return _widthMax; }
	// �c��(min)�̎擾
	inline int GetHeightMin() { return _heightMin; }
	// �c��(max)�̎擾
	inline int GetHeightMax() { return _heightMax; }
	// �����̎擾
	inline int WidhtSize() { return _widthMin + _widthMax; }
	// �c���̎擾
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
	// �R���X�g���N�^(����1:�ړ��x�N�g��(min)�@����2:�ړ��x�N�g��(max)�@����3:����t���O)
	AABB(Vector2 vmin, Vector2 vmax, bool cflag = false);
	// �R���X�g���N�^(����1:���S���W�@����2:�����@����3:�c���@����4:����t���O)
	AABB(Vector2& pos, int width, int height, bool cflag = false);
	// �R���X�g���N�^(����1:���S���W�@����2:����(min)�@����3:����(max)�@����4:����(min)�@����5:����(max)�@����6:����t���O)
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag = false);
	// �f�X�g���N�^
	~AABB() = default;
	// �����蔻��̍X�V(���]�����L��)
	void Update(Vector2& pos, bool inv) override;
};
