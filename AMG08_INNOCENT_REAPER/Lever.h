/*****************************************************************//**
 * \file   Lever.h
 * \brief  ���o�[�N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	namespace gimmick {
		namespace door {
			// �h�A�ԍ�
			constexpr auto NOT_DOOR = -1;	// �Y���Ȃ�
			constexpr auto D_LEVER = 1;		// ���o�[
			constexpr auto D_RED = 2;		// ��
			constexpr auto D_BLUE = 3;		// ��
			constexpr auto D_BOSS = 4;		// �{�X
		}
	}
	// ��d�C���N���[�h�h�~
	class Door;
	// ���o�[�N���X
	class Lever : public GimmickBase {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		Lever(Game& game);
		// �f�X�g���N�^
		~Lever() = default;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
		// �h�A�̉������
		void OpenDoor();
		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue objValue) override;
		// �I�u�W�F�N�g���̍X�V
		void ObjValueUpdate() override;
	private:
		std::shared_ptr<Door> _door;	// ��
	};

}