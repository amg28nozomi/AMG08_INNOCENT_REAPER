/*****************************************************************//**
 * @file   AABB.h
 * @brief  ���]���������铖���蔻��N���X�i�����s���E�{�b�N�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Collision.h"

/** ���]�������L�铖���蔻��N���X */
class AABB : public Collision {
public:
	/**
	 * @brief					�R���X�g���N�^
	 * @param	min			���W�x�N�g��2(min)
	 * @param	max			���W�x�N�g��2(max)
	 * @param	flag		�����蔻����s�����̃t���O
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
	 * @brief					�f�X�g���N�^
	 */
	~AABB() = default;
	/**
	 * @brief					�����蔻��̍X�V(�����ɉ��������]�����L��)
	 * @param pos			���݂̍��W�x�N�g��
	 * @param inv			���݂̌���
	 */
	void Update(Vector2& pos, bool inv) override;
};
