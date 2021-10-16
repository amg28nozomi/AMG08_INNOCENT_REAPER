/*****************************************************************//**
 * @file   Loads.h
 * @brief  �e��f�ނ̓ǂݍ��ݏ������s�����[�h�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Scenario.h"
#include "ImageValue.h"

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class ObjectValue;
	/** �e��f�ނ���уI�u�W�F�N�g���̐������s�����[�h�N���X */
	class Loads {
	public:
		/**
		 * @brief	�e��f�ނ̓ǂݍ���
		 */
		static void ResourceLoad();
		/**
		 * @brief		�摜���̐�������ю擾
		 * @return	�摜�����i�[�����A�z�z���Ԃ�
		 */
		static ImageMap LoadImages();
		/**
		 * @brief		�O�i���̐�������ю擾
		 * @return	�O�i�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ImageValue> LoadForeGround1();
		/**
		 * @brief		�`���[�g���A���摜�̔z�u���̎擾(�`���[�g���A��1)
		 * @return	�`���[�g���A���摜�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<TutorialValue> LoadTutorialF();
		/**
		 * @brief		�`���[�g���A���摜�̔z�u���̎擾(�`���[�g���A��2)
		 * @return	�`���[�g���A���摜�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<TutorialValue> LoadTutorialS();
		/**
		 * @brief		�`���[�g���A���X�e�[�W(����̍Ւd)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenarioS_1();
		/**
		 * @brief		�X�e�[�W0(����̍Ւd)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenarioS();
		/**
		 * @brief		�X�e�[�W1(�r�ꂽ��)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenario1();
		/**
		 * @brief		�X�e�[�W2(�_��̐X-����)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenario2();
		/**
		 * @brief		�X�e�[�W2(�_��̐X-�㕔)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenario2_1();
		/**
		 * @brief		�X�e�[�W2(�_��̐X-����)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenario2_2();
		/**
		 * @brief		�X�e�[�W3(�_��̐X-�{�X)�̃I�u�W�F�N�g�z�u���̐�������ю擾
		 * @return	�I�u�W�F�N�g�����i�[�����R���e�i��Ԃ�
		 */
		static std::vector<ObjectValue> LoadScenarioB();
	};
}

