#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {

	namespace mapchip {
		// �}�b�v�`�b�v�ɓ�����ʂ͂��邩�H
		constexpr auto NORMAL = 0;	// �ʏ�`�b�v(���ʂȂ�)
		constexpr auto THORM = 1;	// ��(�_���[�W���肠��)
		constexpr auto IVY = 2;		// ��(�Ԃ牺����\)

		constexpr auto HIT_ON = true;	// �����蔻��/�����o�������L��
		constexpr auto HIT_OFF = false;	// �����蔻��/�����o����������
	}

	// �}�b�v�̓����蔻����Ǘ�
	class ChipNumber {
	public:
		// �����蔻��ɏC�����Ȃ��ꍇ�͈����w��Ȃ���OK
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);

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

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// �}�b�v�`�b�v�̓����蔻��o�^
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }	// �ǂݍ��ރL�[���̍X�V
		int IsChipType(ChipNumber chipnumber);
		bool IsHitType(ChipNumber chipnumber);	// �Ώۂɓ����蔻��͂��邩�H
		AABB ChipCollision(const int no);	// �Ώۂ̃}�b�v�`�b�v
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _chipKey;	// ���o���}�b�v�`�b�v���(�L�[)
		StageMaps _stageChipsMap;	// �X�e�[�W�̏��

		void ClearStageMaps();
	};
}

