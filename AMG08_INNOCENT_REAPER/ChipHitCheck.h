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
	}

	// �}�b�v�̓����蔻����Ǘ�
	class ChipNumber {
	public:
		// �����蔻��ɏC�����Ȃ��ꍇ�͈����w��Ȃ���OK
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL);
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL);

		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }
		inline int ChipType() { return chipType; }

	private:
		int widthMin;	// ����(Min)
		int widthMax;	// ����(Max)
		int heightMin;	// �c��(Min)
		int heightMax;	// �c��(Max)
		int chipType;	// �`�b�v�̎��
	};

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// �}�b�v�`�b�v�̓����蔻��o�^
		inline void ChangeStageKey(std::string nextkey) { _stageKey = nextkey; }	// �ǂݍ��ރL�[���̍X�V
		AABB ChipCollision(const int no);	// �Ώۂ̃}�b�v�`�b�v
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _stageKey;	// ���݂̃X�e�[�W
		StageMaps _stageChipsMap;	// �X�e�[�W�̏��

		void ClearStageMaps();
	};
}

