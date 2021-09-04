#include "ChipHitCheck.h"
#include "Collision.h"
#include "Vector2.h"

namespace inr {
	ChipNumber::ChipNumber(int width1, int width2, int height1, int height2, int type, bool ishit) {
		widthMin = width1;
		widthMax = width2;
		heightMin = height1;
		heightMax = height2;
		chipType = type;
		hitType = ishit;
	}

	ChipNumber::ChipNumber(int width1, int width2, int type, bool ishit) {
		widthMin = width1;
		widthMax = width2;
		heightMin = 0;
		heightMax = 40;
		chipType = type;
		hitType = ishit;
	}

	ChipHitCheck::ChipHitCheck() {
		_chipKey = "";
		ClearStageMaps();
	}

	ChipHitCheck::~ChipHitCheck() {
		ClearStageMaps();
	}

	void ChipHitCheck::ClearStageMaps() {
		for (auto&& scm : _stageChipsMap) {
			auto& key = scm.first;	// �Q�ƌ��̃L�[�Ō���
			auto chip = _stageChipsMap.find(key);
			chip->second.clear();	// �A�z�z�񏉊���
		}
		_stageChipsMap.clear();
	}

	void ChipHitCheck::LoadChipsMap(std::string key, ChipsMap& chipsMap) {
		// ���̏��͊��ɓo�^����Ă��邩�H
		auto it = _stageChipsMap.find(key);
		// �q�b�g�����ꍇ�͏������甲����
		if (it != _stageChipsMap.end()) return;
		_stageChipsMap.emplace(key, chipsMap);	// �A�z�z��ɓo�^
	}

	AABB ChipHitCheck::ChipCollision(const int no) {
		Vector2 min;
		Vector2 max;

		auto stage = _stageChipsMap.find(_chipKey);	// ���݂̃X�e�[�W�̘A�z�z������o��
		auto chipdata = stage->second.find(no);	// �`�b�v�ԍ��̓����蔻����擾
		if (chipdata == stage->second.end()) {
			// �o�^����Ă��Ȃ������肪�ς���Ă��Ȃ�
			// 40*40�̒ʏ�̒l��߂�l�Ƃ��ĕԂ�
			min = { 0, 0 };
			max = { 40, 40 };
		} else {
			auto chip = chipdata->second;	// �o�^����Ă���ChipNumber���擾

			min = { static_cast<double>(chip.WidthMin()), static_cast<double>(chip.HeightMin()) };
			max = { static_cast<double>(chip.WidthMax()), static_cast<double>(chip.HeightMax()) };
		}
		AABB chipbox = { min, max };
		return chipbox;
	}
}
