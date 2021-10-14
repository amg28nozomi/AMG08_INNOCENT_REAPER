/*****************************************************************//**
 * @file   ChipHitCheck.h
 * @brief  �}�b�v�`�b�v�̓o�^�����Ǘ�����`�b�v�q�b�g�`�F�b�N�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "AABB.h"

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class ChipNumber;
	/** �}�b�v�`�b�v�̓����蔻����Ǘ����� */
	class ChipHitCheck {
	public:
		/** �`�b�v�̔�������Ǘ�����A�z�z��(�L�[:�`�b�v�ԍ��@�l:�Ή�����`�b�v�̔�����) */
		using ChipsMap = std::unordered_map<int, ChipNumber>;
		/**
		 * @brief	�R���X�g���N�^
		 */
		ChipHitCheck();
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ChipHitCheck();
		/**
		 * @brief						�}�b�v�`�b�v�̓����蔻��o�^
		 * @param key				�o�^�p�L�[
		 * @param chipsMap	�`�b�v�����i�[�����A�z�z��
		 */
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		/**
		 * @brief					���o���p�L�[���̍X�V
		 * @param nextkey	�؂�ւ��p�L�[
		 */
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		/**
		 * @brief			�Ώۂ̃}�b�v�`�b�v�Ɍ��ʂ����邩�̔���
		 * @param no	������s���`�b�v�ԍ�
		 * @return		�Ή�������ʔԍ���Ԃ�
		 *						�`�b�v�ԍ����Y���Ȃ��̏ꍇ��-1��Ԃ�
		 */
		int IsChipType(const int no);
		/**
		 * @brief			�Ώۂɓ����蔻�肪���邩�̔���
		 * @param no	������s���`�b�v�ԍ�
		 * @return		���肪����ꍇ��true��Ԃ�
		 *						���肪�Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsHitType(const int no);
		/**
		 * @brief			�Ώۂ̓����蔻��{�b�N�X���擾
		 * @param no	������s���`�b�v�ԍ�
		 * @return		�q�b�g�����ꍇ�͎擾���������蔻��{�b�N�X��Ԃ�
		 *						�o�^����Ă��Ȃ��ꍇ��40*40�̓����蔻��{�b�N�X��Ԃ�
		 */
		AABB ChipCollision(const int no);
	private:
		/** �}�b�v�`�b�v�̔�������Ǘ�����A�z�z��(�L�[:�`�b�v�L�[�@�l:�}�b�v�`�b�v�̔�������Ǘ�����R���e�i) */
		using StageMaps = std::unordered_map<std::string, ChipsMap>;
		std::string _chipKey;				//!< ���o���}�b�v�`�b�v���(�L�[)
		StageMaps _stageChipsMap;		//!< �X�e�[�W�̏��
		/**
		 * @brief	�o�^���̏�����
		 */
		void ClearStageMaps();
	};
}