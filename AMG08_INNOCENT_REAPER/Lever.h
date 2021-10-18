/*****************************************************************//**
 * @file   Lever.h
 * @brief  ���o�[�N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	/** �M�~�b�N�̃R���p�C�����萔 */
	namespace gimmick {
		/** �h�A�̃R���p�C�����萔 */
		namespace door {
			// �h�A�ԍ�
			constexpr auto NOT_DOOR = -1;	// �Y���Ȃ�
			constexpr auto D_LEVER = 1;		// ���o�[
			constexpr auto D_RED = 2;			// ��
			constexpr auto D_BLUE = 3;		// ��
			constexpr auto D_BOSS = 4;		// �{�X
		}
	}
	/** ��d�C���N���[�h�h�~ */
	class Door;
	/** ���o�[ */
	class Lever : public GimmickBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 */
		Lever(Game& game);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~Lever() = default;
		/**
		 * @brief						�X�V����
		 */
		void Process() override;
		/**
		 * @brief						�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief						�h�A�̊J������
		 */
		void OpenDoor();
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief						�I�u�W�F�N�g���̍X�V
		 */
		void ObjValueUpdate() override;
	private:
		std::shared_ptr<Door> _door;	//!< ��
	};

}