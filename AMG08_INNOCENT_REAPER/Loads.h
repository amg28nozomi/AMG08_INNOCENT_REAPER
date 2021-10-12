/*****************************************************************//**
 * \file   Loads.h
 * \brief  ���[�h�N���X
 *		   �e��f�ނ̓ǂݍ��ݏ������s��
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Scenario.h"
#include "ImageValue.h"

namespace inr {
	class ObjectValue;	// ��d�C���N���[�h�h�~
	// �e��f�ނ���уI�u�W�F�N�g���̐������s���N���X
	class Loads {
	public:
		// �e��f�ނ̓ǂݍ���
		static void ResourceLoad();
		// �摜�������̐�������ю擾
		static ImageMap LoadImages();
		// �O�i���̐�������ю擾
		static std::vector<ImageValue> LoadForeGround1();
		// �`���[�g���A���摜�̔z�u���̎擾(�`���[�g���A��1)
		static std::vector<TutorialValue> LoadTutorialF();
		// �`���[�g���A���摜�̔z�u���̎擾(�`���[�g���A��2)
		static std::vector<TutorialValue> LoadTutorialS();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �`���[�g���A��1(����̍Ւd)
		static std::vector<ObjectValue> LoadScenarioS_1();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �`���[�g���A��2(����̍Ւd)
		static std::vector<ObjectValue> LoadScenarioS();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �X�e�[�W1(�r�ꂽ��)
		static std::vector<ObjectValue> LoadScenario1();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �X�e�[�W2(�_��̐X-����)
		static std::vector<ObjectValue> LoadScenario2();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �X�e�[�W2(�_��̐X-�㕔)
		static std::vector<ObjectValue> LoadScenario2_1();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �X�e�[�W2(�_��̐X-����)
		static std::vector<ObjectValue> LoadScenario2_2();
		// �I�u�W�F�N�g�̔z�u���̐�������ю擾
		// �X�e�[�W3(�_��̐X-�{�X)
		static std::vector<ObjectValue> LoadScenarioB();
	};
}

