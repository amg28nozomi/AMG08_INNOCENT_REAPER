/*****************************************************************//**
 * @file   Door.h
 * @brief  �h�A�N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	/** �h�A�M�~�b�N */
	class Door : public GimmickBase {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Door(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Door() = default;
		/**
		 * @brief	����������
		 */
		void Init() override;
		/**
		 * @brief	�X�V����
		 */
		void Process() override;
		/**
		 * @brief	�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief				�I�u�W�F�N�g���̓o�^
		 * @param spwan	�������W
		 * @param key		�摜�L�[
		 * @param flag	�M�~�b�N�t���O
		 */
		void SetParameter(Vector2 spwan, std::string key, int flag);
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief	�J�t���O�̋N��
		 */
		void SwitchOn();
		/**
		 * @brief	�J�t���O�̗}��
		 */
		void SwitchOff();
		/**
		 * @brief						�I�u�W�F�N�g�̉����o������
		 * @param	box				�Ώۂ̓����蔻��{�b�N�X
		 * @param	pos				�Ώۂ̍��W�x�N�g��(�Q��)
		 * @param	move			�Ώۂ̈ړ��x�N�g��(�Q��)
		 * @param	direction	�Ώۂ̌����t���O
		 * @return					�Փ˂��Ă���ꍇ��true��Ԃ�
		 *									�Փ˂��Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction);
		/**
		 * @brief		���̐F�̎擾
		 * @return	���̐F��Ԃ�
		 */
		inline int DoorColor() { return _color; }
		/**
		 * @brief		�J�t���O�̎擾
		 * @return	�J�t���O��Ԃ� 
		 */
		inline bool IsSwitch() { return _switch; }
	private:
		int _color;					//!< �h�A�̐F
		double _normalY;		//!< �ʏ���W
		bool _switch;				//!< �J�t���O
		bool _ismove;				//!< �ړ������H
		Vector2 _moves;			//!< �ړ����W
		/**
		 * @brief			���̐F�̐ݒ�
		 * @param key	�摜�L�[
		 */
		void SetColor(std::string key);
		/**
		 * @brief		�A�j���[�V��������
		 * @return	�A�j���[�V�����̍Đ����s���ꍇ��true��Ԃ�
		 *					�Đ����s��Ȃ��ꍇ��false��Ԃ�
		 */
		bool MotionCount();
		/**
		 * @brief		�ړ�����
		 * @return 
		 */
		bool DoorMove();
	};
}

