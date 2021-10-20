/*****************************************************************//**
 * @file   Block.h
 * @brief  �����N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	/** �M�~�b�N�̃R���p�C�����萔 */
	namespace gimmick {
		/** �����̃R���p�C�����萔 */
		namespace block {
			constexpr auto BRAKE_ON = true;			//!< �j�󂳂ꂽ
			constexpr auto BRAKE_OFF = false;		//!< �j�󂳂�Ă��Ȃ�
		}
	}
	/** �����M�~�b�N */
	class Block : public GimmickBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param	game			�Q�[���N���X�̎Q��
		 */
		Block(Game& game);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~Block() = default;
		/**
		 * @brief						�X�V����
		 */
		void Process() override;
		/**
		 * @brief						�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param	objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief						�I�u�W�F�N�g���̍X�V
		 */
		void ObjValueUpdate() override;
		/**
		 * @brief						�I�u�W�F�N�g�͉��Ă��邩�H
		 * @return					���Ă���ꍇ��true��Ԃ�
		 *									���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		inline bool IsBreak() { return _break; }
		/**
		 * @brief						�j��\�t���O�̎擾
		 * @return					�j��\�t���O��Ԃ�
		 */
		inline bool IsInvalid() { return _invalid; }
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
		 * @brief						�j�󏈗�
		 */
		void Break();
		/**
		 * @brief						�I�u�W�F�N�g�̔j����s���Ȃ�����
		 */
		inline void SetInvalid() { _invalid = true; }
	private:
		int _pal;				//!< �����x
		bool _break;		//!< �󂳂ꂽ���ۂ�
		bool _invalid;	//!< ���̃I�u�W�F�N�g�͔j��\��
	};
}

