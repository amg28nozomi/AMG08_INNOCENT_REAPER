/*****************************************************************//**
 * @file   GimmickBase.h
 * @brief  �M�~�b�N�̃X�[�p�[�N���X�i�I�u�W�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"

namespace inr {
	/** �M�~�b�N�̃R���p�C�����萔 */
	namespace gimmick {
		// �M�~�b�N�ԍ�
		constexpr auto NOT_GIMMICK = -1;	//!< �Y���Ȃ�
		constexpr auto LEVER = 0;					//!< ���o�[
		constexpr auto CRYSTAL = 1;				//!< ����
		constexpr auto BLOCK = 2;					//!< �����
		constexpr auto DOOR = 3;					//!< �h�A
		// �M�~�b�N�̃t���O
		constexpr auto OFF = false;				//!< �M�~�b�N�I�t
		constexpr auto ON = true;					//!< �M�~�b�N�I��
		/** ���o�[�̃R���p�C�����萔 */
		namespace lever {
			constexpr auto KEY_LEVER = "lever";						//!< �ʏ탌�o�[
			constexpr auto KEY_LEVER_BOSS = "boss_lever";	//!< �{�X���p���o�[
			constexpr auto LEVER_SIZE = 140;							//!< �ʏ탌�o�[�̉摜�T�C�Y
			constexpr auto BOSS_LEVER_SIZE = 200;					//!< �{�X���p���o�[�̉摜�T�C�Y
			// �T�E���h�Đ��Ԋu
			constexpr auto SE_LEVER = 50;									//!< SE�̍Đ��Ԋu
		}
		/** �����̃R���p�C�����萔 */
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";				//!< �����
			constexpr auto KEY_BREAK = "break_key";				//!< �j��G�t�F�N�g
			// �摜�T�C�Y(��)
			constexpr auto BLOCK_SIZE_W = 140;						//!< �����̕�
			constexpr auto BLOCK_SIZE_H = 280;						//!< �����̍���
			// �摜�T�C�Y(�G�t�F�N�g)
			constexpr auto BRAKE_SIZE = 500;							//!< �j��G�t�F�N�g�̃T�C�Y
			// �T�E���h
			constexpr auto SE_BREAK = 50;									//!< SE�̍Đ��Ԋu
		}
		/** �����̃R���p�C�����萔 */
		namespace crystal {
			/** �ʏ� */
			namespace type1 {
				// �摜�L�[
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal1_empty";	//!< �ʏ퐅��(��)
				constexpr auto KEY_CRYSTAL_RRD = "crystal1_red";			//!< �ʏ퐅��(��)
				constexpr auto KEY_CRYSTAL_BLUE = "crystal1_blue";		//!< �ʏ퐅��(��)
			}
			/** �X�p */
			namespace type2 {
				// �摜�L�[
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal2_empty";	//!< �X�p����(��)
				constexpr auto KEY_CRYSTAL_RED = "crystal2_red";			//!< �X�p����(��)
				constexpr auto KEY_CRTSTAL_BLUE = "crystal2_blue";		//!< �X�p����(��)
			}
			constexpr auto CRYSTAL_SIZE = 140;											//!< �����̃T�C�Y
			// �T�E���h
			constexpr auto KEY_CRYSTAL = "crystal_se";							//!< ����SE
			constexpr auto SE_CRYSTAL = 40;													//!< SE�̍Đ��Ԋu
			// SE
			constexpr auto KEY_ROB = "crystal_rob_se";							//!< ����D��ꂽ
			constexpr auto KEY_GIVE = "crystal_give_se";						//!< ����^����ꂽ
		}
		/** �h�A�̃R���p�C�����萔 */
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";						//!< ���o�[��
			constexpr auto KEY_DOOR_BOSS = "door_boss";							//!< �{�X��
			constexpr auto KEY_DOOR_RED = "door_red";								//!< �Ԕ�
			constexpr auto KEY_DOOR_BLUE = "door_blue";							//!< ��
			constexpr auto DOOR_SIZE = 280;													//!< �h�A�̃T�C�Y
			// �T�E���h
			constexpr auto KEY_DOOR = "door_se";										//!< �J����
			constexpr auto SE_OPEN = 40;														//!< SE�̍Đ��Ԋu
			constexpr auto SE_CLOSE_DOOR = "door_close";						//!< ����
		}
	}
	/** �M�~�b�N�̃X�[�p�[�N���X */
	class GimmickBase : public ObjectBase {
	protected:
		/**
		 * @brief �M�~�b�N�̎�ނ�\���񋓌^
		 *				�Y���Ȃ��A���o�[�A��A�����A�h�A
		 */
		enum class GimmickType {
			NOT_GIMMICK, LEVER, BLOCK, CRYSTAL, DOOR
		};
		GimmickType _gType;		//!< �M�~�b�N�̎��
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		GimmickBase(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~GimmickBase();
		/**
		 * @brief				����������
		 * 
		 */
		virtual void Init() override;
		/**
		 * @brief				�X�V����
		 * 
		 */
		virtual void Process() override;
		/**
		 * @brief				�`�揈��
		 * @brief				�`�揈��
		 */
		virtual void Draw() override;
		/**
		 * @brief				�I�u�W�F�N�g���̍X�V
		 */
		virtual void ObjValueUpdate();
		/**
		 * @brief				�M�~�b�N�ԍ��̎擾
		 * @return			�M�~�b�N�ԍ���Ԃ�
		 */
		int GimmickType();
	};
}

