/*****************************************************************//**
 * @file   Collision.h
 * @brief  �����蔻��N���X
 *		   �����s���E�{�b�N�X�N���X�i�����蔻��N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include "Vector2.h"

/**   �����蔻��̊��N���X	*/
class Collision {
protected:
	Vector2 _minV;				//!< 2D�x�N�g��(Min)
	Vector2 _maxV;				//!< 2D�x�N�g��(Max)
	Vector2 _center;			//!< ���S���W
	int _widthMin;				//!< ��min
	int _widthMax;				//!< ��max
	int _heightMin;				//!< ����min
	int _heightMax;				//!< ����max
	bool _collisionFalg;	//!< ���肷�邩�ǂ����̃t���O
#ifdef _DEBUG
	bool _drawFlag;				// �`�悷�邩�ǂ���
#endif
public:
	/**
	 * @brief					�R���X�g���N�^
	 * @param pos			���S���W
	 * @param width1	����(min)
	 * @param width2	����(max)
	 * @param height1	�c��(min)
	 * @param height2	�c��(max)
	 * @param flag		�����蔻����s�����̃t���O
	 */
	Collision(Vector2& pos, int width1, int width2, int height1, int height2, bool flag = false);
	/**
	 * @brief					�R���X�g���N�^
	 * @param pos			���S���W
	 * @param width		����
	 * @param height	�c��
	 * @param flag		�����蔻����s�����̃t���O
	 */
	Collision(Vector2& pos, int width, int height, bool flag = false);
	/**
	 * @brief				�R���X�g���N�^
	 * @param	min		���W�x�N�g��2(min)
	 * @param	max		���W�x�N�g��2(max)
	 * @param	flag	�����蔻����s�����̃t���O
	 */
	Collision(Vector2& min, Vector2& max, bool flag = false);
	/**
	 * @brief	�R���X�g���N�^
	 */
	Collision();
	/**
	 * @brief	�f�X�g���N�^
	 */
	~Collision() = default;
	/**
	 * @brief		2D�x�N�g��(min)�̎擾
	 * @return	2D�x�N�g��(min)��Ԃ�
	 */
	inline Vector2 GetMin() { return _minV; }
	/**
	 * @brief		2D�x�N�g��(max)�̎擾
	 * @return	2D�x�N�g��(max)��Ԃ�
	 */
	inline Vector2 GetMax() { return _maxV; }
	/**
	 * @brief		���S���W�̎擾
	 * @return	���S���W��Ԃ�
	 */
	inline Vector2 GetCenter() { return _center; }
	/**
	 * @brief		����(min)�̎擾
	 * @return	����(min)��Ԃ�
	 */
	inline int GetWidthMin() { return _widthMin; }
	/**
	 * @brief		����(max)�̎擾 
	 * @return	����(max)��Ԃ�
	 */
	inline int GetWidthMax() { return _widthMax; }
	/**
	 * @brief		�c��(min)�̎擾
	 * @return	�c��(min)��Ԃ�
	 */
	inline int GetHeightMin() { return _heightMin; }
	/**
	 * @brief		�c��(max)�̎擾
	 * @return	�c��(max)��Ԃ�
	 */
	inline int GetHeightMax() { return _heightMax; }
	/**
	 * @brief		�����̎擾
	 * @return	������Ԃ�
	 */
	inline int WidhtSize() { return _widthMin + _widthMax; }
	/**
	 * @brief		�c���̎擾
	 * @return	�c����Ԃ�
	 */
	inline int HeightSize() { return _heightMin + _heightMax; }
	/**
	 * @brief			�����蔻��̍X�V
	 * @param pos	���݂̈ړ��x�N�g��
	 * @param inv	���݂̌���
	 */
	virtual void Update(Vector2& pos, bool inv);
	/**
	 * @brief			�ꎞ�I�ɓ����蔻���������
	 * @param col	������铖���蔻��
	 */
	virtual void Swap(Collision col);
	/**
	 * @brief				�{�b�N�X�̕`��
	 * @param color	�`��{�b�N�X�̐F(�����ɂ��F�w��Ȃ��̏ꍇ�͔�)
	 */
	void DrawColorBox(int color = GetColor(255, 255, 255));
	/**
	 * @brief						�����蔻�蓯�m�̏Փ˔���
	 * @param collision	�Փ˔�����s���Ώ�
	 * @return					�Փ˂����ꍇ��true��Ԃ�
	 * @return					�Փ˂��Ă��Ȃ��ꍇ��false��Ԃ�
	 */
	bool HitCheck(Collision collision);
#ifdef _DEBUG
	/**
	 * @brief		�`��t���O�̎擾
	 * @return	�`��t���O��Ԃ�
	 */
	inline bool GetDrawFlag() { return _drawFlag; }
	/**
	 * @brief		�`��t���O�̎Q�Ƃ��擾
	 * @return	�`��t���O�̎Q�Ƃ�Ԃ�
	 */
	inline bool& SetDrawFlag() { return _drawFlag; }
#endif
	/**
	 * @brief		����t���O�̎擾
	 * @return	����t���O��Ԃ�
	 */
	inline bool GetCollisionFlag() { return _collisionFalg; }
	/**
	 * @brief		����t���O�̎Q�Ƃ��擾
	 * @return	����t���O�̎Q�Ƃ�Ԃ�
	 */
	inline bool& SetCollisionFlag() { return _collisionFalg; }
};
/** ���]�������L�铖���蔻��N���X */
class AABB : public Collision {
public:
	/**
	 * @brief				�R���X�g���N�^
	 * @param	min		���W�x�N�g��2(min)
	 * @param	max		���W�x�N�g��2(max)
	 * @param	flag	�����蔻����s�����̃t���O
	 */
	AABB(Vector2 vmin, Vector2 vmax, bool cflag = false);
	/**
	 * @brief					�R���X�g���N�^
	 * @param pos			���S���W
	 * @param width		����
	 * @param height	�c��
	 * @param flag		�����蔻����s�����̃t���O
	 */
	AABB(Vector2& pos, int width, int height, bool cflag = false);
	/**
	 * @brief					�R���X�g���N�^
	 * @param pos			���S���W
	 * @param width1	����(min)
	 * @param width2	����(max)
	 * @param height1	�c��(min)
	 * @param height2	�c��(max)
	 * @param flag		�����蔻����s�����̃t���O
	 */
	AABB(Vector2& vpos, int width1, int width2, int height1, int height2, bool cflag = false);
	/**
	 * @brief	�f�X�g���N�^
	 */
	~AABB() = default;
	// �����蔻��̍X�V(���]�����L��)
	/**
	 * @brief			�����蔻��̍X�V(�����ɉ��������]�����L��)
	 * \param pos	���݂̍��W�x�N�g��
	 * \param inv	���݂̌���
	 */
	void Update(Vector2& pos, bool inv) override;
};
