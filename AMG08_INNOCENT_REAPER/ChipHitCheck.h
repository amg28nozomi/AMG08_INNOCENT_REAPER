/*****************************************************************//**
 * \file   ChipHitCheck.h
 * \brief  �`�b�v�ԍ��N���X
 *		   �}�b�v�`�b�v�̓o�^���
 * 
 *		   �`�b�v�q�b�g�`�F�b�N�N���X
 *		   �}�b�v�`�b�v�̓����蔻����Ǘ�����
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
	namespace mapchip {
		// �}�b�v�`�b�v�ɓ�����ʂ͂��邩�H
		constexpr auto NONE = -1;		// �`�b�v����
		constexpr auto NORMAL = 0;		// �ʏ�`�b�v(���ʂȂ�)
		constexpr auto THORM = 1;		// ��(�_���[�W���肠��)
		constexpr auto IVY = 2;			// ��(�Ԃ牺����\)
		constexpr auto TRANSITION = 3;	// �X�e�[�W�J��
		// �����蔻��͂��邩�H
		constexpr auto HIT_ON = true;	// �����蔻��/�����o�������L��
		constexpr auto HIT_OFF = false;	// �����蔻��/�����o����������
	}
	// �}�b�v�`�b�v�̓����蔻��
	class ChipNumber {
	public:
		// �R���X�g���N�^(����1:����(min)�@����2:����(max)�@����3:�c��(min)�@����4:�c��(max)�@����5:�}�b�v�`�b�v�̌��ʁ@����6:�����蔻��͂��邩)
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		// �R���X�g���N�^(����1:����(min)�@����2:����(max)�@����3:�}�b�v�`�b�v�̌��ʁ@����4:�����蔻��t���O)
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		// �R���X�g���N�^(����1:�}�b�v�`�b�v�̌��ʁ@����2:�����蔻��t���O)
		ChipNumber(int type, bool ishit = mapchip::HIT_ON);
		// ����(min)�̎擾
		inline int WidthMin() { return widthMin; }
		// ����(max)�̎擾
		inline int WidthMax() { return widthMax; }
		// �c��(min)�̎擾
		inline int HeightMin() { return heightMin; }
		// �c��(max)�̎擾
		inline int HeightMax() { return heightMax; }
		// �`�b�v�̎�ނ̎擾
		inline int ChipType() { return chipType; }
		// �����蔻��t���O�̎擾
		inline bool HitType() { return hitType; }
	private:
		int widthMin;	// ����(Min)
		int widthMax;	// ����(Max)
		int heightMin;	// �c��(Min)
		int heightMax;	// �c��(Max)
		int chipType;	// �`�b�v�̎��
		bool hitType;	// �����蔻��͂��邩
	};
	// �}�b�v�`�b�v�̓����蔻����Ǘ�����
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

