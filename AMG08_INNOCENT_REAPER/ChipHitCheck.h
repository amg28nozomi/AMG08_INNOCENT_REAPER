/*****************************************************************//**
 * \file   ChipHitCheck.h
 * \brief  �`�b�v�ԍ��N���X
 * \brief	 �}�b�v�`�b�v�̓o�^���
 * \brief	 �`�b�v�q�b�g�`�F�b�N�N���X
 * \brief  �}�b�v�`�b�v�̓����蔻����Ǘ�����
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class ChipNumber;
	/** �}�b�v�`�b�v�̓����蔻����Ǘ����� */
	class ChipHitCheck {
	public:
		// �`�b�v�����i�[����A�z�z��(�L�[:�`�b�v�ԍ��@�l:�}�b�v�`�b�v�̓����蔻��)
		using ChipsMap = std::unordered_map<int, ChipNumber>;
		// �R���X�g���N�^
		ChipHitCheck();
		// �f�X�g���N�^
		~ChipHitCheck();
		// �}�b�v�`�b�v�̓����蔻��o�^(����1:�L�[�@����2:�`�b�v�����i�[�����R���e�i)
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		// �ǂݍ��ރL�[���̍X�V(����:�؂�ւ��p�L�[)
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		// �ΏۂɌ��ʂ����邩�̔���(����:������s���`�b�v�ԍ�)
		int IsChipType(const int no);
		// �Ώۂɓ����蔻�肪���邩�̔���(����:������s���`�b�v�ԍ�)
		bool IsHitType(const int no);
		// �Ώۂ̓����蔻����擾(����:�擾���s���`�b�v�ԍ�)
		AABB ChipCollision(const int no);
	private:
		// (�L�[:�X�e�[�W�L�[�@�l:�Ή��X�e�[�W�̓����蔻��)
		using StageMaps = std::unordered_map<std::string, ChipsMap>;
		std::string _chipKey;		// ���o���}�b�v�`�b�v���(�L�[)
		StageMaps _stageChipsMap;	// �X�e�[�W�̏��
		// �o�^���̏�����
		void ClearStageMaps();
	};
}

