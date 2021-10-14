/*****************************************************************//**
 * \file   Block.h
 * \brief  �����N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	namespace gimmick {
		namespace block {
			// ��ꂽ���ǂ����̃t���O
			constexpr auto BRAKE_ON = true;		// �j��ς�
			constexpr auto BRAKE_OFF = false;	// ���j��
		}
	}
	// �����(�M�~�b�N)
	class Block : public GimmickBase {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		Block(Game& game);
		// �f�X�g���N�^
		~Block() = default;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
		// �I�u�W�F�N�g���̓o�^(����:�I�u�W�F�N�g���)
		void SetParameter(ObjectValue objValue) override;
		// �I�u�W�F�N�g���̍X�V
		void ObjValueUpdate() override;
		// �I�u�W�F�N�g�͉��Ă��邩�H
		inline bool IsBreak() { return _break; }
		// �����o������(����1:�Ώۂ̓����蔻��{�b�N�X�@����2:�Ώۂ̍��W�@����3:�Ώۂ̈ړ��ʁ@����4:�Ώۂ̌���)
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction);
		// �j�󏈗�
		bool Break();
	private:
		int _pal;		// �����x
		bool _break;	// �󂳂ꂽ���ۂ�
	};
}

