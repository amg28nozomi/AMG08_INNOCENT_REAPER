#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

namespace inr {

	// �}�b�v�̓����蔻����Ǘ�
	class ChipNumber {
	public:
		// �����蔻��ɏC�����Ȃ��ꍇ�͈����w��Ȃ���OK
		ChipNumber(int width1, int width2, int height1, int height2);
		ChipNumber(int width1, int width2);

		inline int WidthMin() { return widthMin; }
		inline int WidthMax() { return widthMax; }
		inline int HeightMin() { return heightMin; }
		inline int HeightMax() { return heightMax; }

	private:
		int widthMin;	// ����(Min)
		int widthMax;	// ����(Max)
		int heightMin;	// �c��(Min)
		int heightMax;	// �c��(Max)
	};

	class ChipHitCheck {
	public:
		using ChipsMap = std::unordered_map<int, ChipNumber>;

		ChipHitCheck();
		~ChipHitCheck();

		void ClearStageMaps();
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);	// �}�b�v�`�b�v�̓����蔻��o�^
		inline void ChangeStageKey(std::string nextkey) { _stageKey = nextkey; }	// �ǂݍ��ރL�[���̍X�V
		AABB ChipCollision(const int no);	// �Ώۂ̃}�b�v�`�b�v
	private:
		using StageMaps = std::unordered_map<std::string, ChipsMap>;

		std::string _stageKey;	// ���݂̃X�e�[�W
		StageMaps _stageChipsMap;	// �X�e�[�W�̏��
	};
}

