/*****************************************************************//**
 * @file   LoopEffect.h
 * @brief  ���[�v�G�t�F�N�g�i�G�t�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class ObjectBase;
	/** �G�t�F�N�g(�������[�v�����L��) */
	class LoopEffect : public EffectBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 * @param gh				�摜�L�[
		 * @param spawnpos	�����n�_
		 * @param maxFrame	�����t���[����
		 * @param direction	���]�t���O
		 */
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~LoopEffect() = default;
		/**
		 * @brief						�X�V����
		 */
		void Process() override;
		/**
		 * @brief						���L�҂̐ݒ�
		 * @param						ObjectBase�̃|�C���^
		 */
		void SetOwner(ObjectBase* owner);
	private:
		ObjectBase* _owner;		//!< ���L��
		/**
		 * @brief		�I�u�W�F�N�g���폜���邩�̔���
		 * @return	��������ꍇ��true��Ԃ�
		 *					�������Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsEnd();
		/**
		 * @brief		�Ǐ]����
		 * @return	�����ɐ��������ꍇ��true��Ԃ�
		 *					�����Ɏ��s�����ꍇ��false��Ԃ�
		 */
		bool Move();
	};
}

