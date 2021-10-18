/*****************************************************************//**
 * @file   MoveImage.h
 * @brief  �ړ��������s�����[�u�摜�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"

namespace inr {
	/** �ړ������̂���摜 */
	class MoveImage : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		MoveImage(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~MoveImage() = default;
		/**
		 * @brief				����������
		 */
		void Init() override;
		/**
		 * @brief				�X�V����
		 */
		void Process() override;
		/**
		 * @brief				
		 */
		void Draw() override;
		/**
		 * @brief				�p�����[�^�̐ݒ�
		 * @param gkey	�摜�p�L�[
		 * @param pos		���������n�_
		 */
		void SetParameter(const std::string gkey, Vector2 pos);
		/**
		 * @brief				�ړ������t���O�̎擾
		 * @return			�ړ������t���O��Ԃ�
		 */
		inline bool IsActive() { return _isMove; }
	private:
		bool _isMove;		//!< �ړ��������s�����H
		Vector2 _start;	//!< �������W
		/**
		 * @brief				�ړ�����
		 * @return			�ړ����I������ꍇ��true��Ԃ�
		 *							�ړ����p������ꍇ��false��Ԃ�
		 */
		bool Move();
	};
}

