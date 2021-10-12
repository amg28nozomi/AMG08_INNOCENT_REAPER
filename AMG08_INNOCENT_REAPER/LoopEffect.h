/*****************************************************************//**
 * \file   LoopEffect.h
 * \brief  ���[�v�G�t�F�N�g�i�G�t�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {
	// ��d�C���N���[�h�h�~
	class ObjectBase;
	// �������[�v�����L��
	class LoopEffect : public EffectBase {
	public:
		// �R���X�g���N�^(����1:�Q�[���N���X�̎Q�Ɓ@����2:�摜�L�[�@����3:�����n�_�@����4:�����t���[�����@����5:���]�t���O)
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		// �f�X�g���N�^
		~LoopEffect() = default;
		// �X�V
		void Process() override;
		// ���L�҂̐ݒ�(����:ObjectBase�^�̃|�C���^)
		void SetOwner(ObjectBase* owner);
	private:
		ObjectBase* _owner;		// ���L��
		// �I�u�W�F�N�g���폜���邩�̔���
		bool IsEnd();
		// �Ǐ]����
		bool Move();
	};
}

