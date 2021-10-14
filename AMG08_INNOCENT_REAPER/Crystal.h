/*****************************************************************//**
 * @file   Crystal.h
 * @brief  �����N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"
#include "SoulSkin.h"
#include <memory>
#include <vector>

namespace inr {
	/** ���d�C���N���[�h�h�~ */
	class Door;
	/** �����M�~�b�N */
	class Crystal : public GimmickBase {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Crystal(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Crystal() = default;
		/**
		 * @brief	�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief �I�u�W�F�N�g���̍X�V
		 */
		void ObjValueUpdate() override;
		/**
		 * @brief							���@�A�N�V�����{�b�N�X�Ƃ̏Փ˔���
		 * @param	ckey				�Ώۂ̌��݂̏��(�L�[)
		 * @param	acollision	�Ώۂ̓����蔻��{�b�N�X
		 * @param	direction		�Ώۂ̌����t���O
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction);
	private:
		std::shared_ptr<SoulSkin> _soul;						//!< �ێ����Ă��鍰
		std::vector<std::shared_ptr<Door>> _doors;	//!< �ێ����Ă���h�A
		/**
		 * @brief			�h�A�̐ݒ�
		 * @param key	���̉摜�L�[���i�[���ꂽ�R���e�i
		 */
		void SetDoors(std::vector<std::string> key);
		/**
		 * @brief	�摜�L�[�̐؂�ւ�
		 */
		void GraphKey();
		/**
		 * @brief	���̐�������
		 */
		void AddSoul();
		/**
		 * @brief				�h�A�ƌ��ݕۗL���Ă��鍰�̐F�͈�v���Ă��邩
		 * @param type	�h�A�̐F
		 * @return			��v���Ă���ꍇ��true��Ԃ�
		 *							�s��v�̏ꍇ��false��Ԃ�
		 */
		bool IsOpen(int type);
	};
}

