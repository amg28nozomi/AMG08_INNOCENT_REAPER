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
		// �����蔻��ɏC�����Ȃ��ꍇ�͈����w��Ȃ�
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		ChipNumber(int type, bool ishit = mapchip::HIT_ON);

		// �Q�b�^�[
		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }
		inline int ChipType() { return chipType; }
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
		// �A�z�z��(����:�`�b�v�ԍ��@�E��:�}�b�v�`�b�v�̓����蔻��)
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		// �}�b�v�`�b�v�̓����蔻��o�^
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		// �ǂݍ��ރL�[���̍X�V
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		// �ΏۂɌ��ʂ����邩�̔���(����:������s���`�b�v�ԍ�)
		int IsChipType(const int no);
		// �Ώۂɓ����蔻�肪���邩�̔���(����:������s���`�b�v�ԍ�)
		bool IsHitType(const int no);
		// �Ώۂ̓����蔻����擾(����:�擾���s���`�b�v�ԍ�)
		AABB ChipCollision(const int no);
	private:
		// �A�z�z��(����:�X�e�[�W�L�[�@�E��:�Ή��X�e�[�W�̓����蔻��)
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _chipKey;		// ���o���}�b�v�`�b�v���(�L�[)
		StageMaps _stageChipsMap;	// �X�e�[�W�̏��
		// �o�^���̏�����
		void ClearStageMaps();
	};
}

