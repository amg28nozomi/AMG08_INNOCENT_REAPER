/*****************************************************************//**
 * @file   TrackingEffect.h
 * @brief  �ǐՃG�t�F�N�g�i�G�t�F�N�g�x�[�X�̃T�u�N���X�j
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
	/** �ǐՃG�t�F�N�g */
	class TrackingEffect : public EffectBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 * @param gh				�摜�L�[
		 * @param spawnpos	�������W
		 * @param maxFrame	�����t���[����
		 * @param direction	����
		 */
		TrackingEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~TrackingEffect() = default;
		/**
		 * @brief						�X�V����
		 */
		void Process() override;
		/**
		 * @brief						�Ǐ]�����̐ݒ�
		 * \param target		�Ǐ]�Ώ�
		 * \param fixx			�C���l(x)
		 * \param fixy			�C���l(y
		 */
		void Set(ObjectBase* target, double fixx = 0, double fixy = 0);
	private:
		ObjectBase* _target;	//!< �ǐՖڕW
		Vector2 _fix;					//!< �C����
	};
}

