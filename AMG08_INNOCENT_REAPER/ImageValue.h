/*****************************************************************//**
 * @file   ImageValue.h
 * @brief  �C���[�W�摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include "ParticleValue.h"
#include <string>
#include <vector>

namespace inr {
	/** �摜��� */
	class ImageValue {
	public:
		/**
		 * @brief								�R���X�g���N�^
		 */
		ImageValue();
		/**
		 * @brief								�R���X�g���N�^
		 * @param gkey					�摜�L�[
		 * @param pos						�������W
		 * @param paricleValue	�p�[�e�B�N���摜�̏��
		 */
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		/**
		 * @brief								�f�X�g���N�^
		 */
		~ImageValue() = default;
		/**
		 * @brief								�摜�L�[�̎擾
		 * @return							�摜�L�[�̕Ԃ�
		 */
		inline std::string GraphKey() { return _graph; }
		/**
		 * @brief								�������W�̎擾
		 * @return							�������W��Ԃ�
		 */
		inline Vector2 Position() { return _position; }
		/**
		 * @brief								�p�[�e�B�N���摜���̎擾
		 * @return							�p�[�e�B�N���摜����Ԃ�
		 */
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;						//!< �摜�L�[
		Vector2 _position;						//!< ���W
		ParticleValue _particleValue;	//!< �p�[�e�B�N���C���[�W�̏��
	};
}

