#include "ChipHitCheck.h"
#include "Collision.h"
#include "Vector2.h"

namespace inr {
	ChipNumber::ChipNumber(int width1, int width2, int height1, int height2) {
		widthMin = width1;
		widthMax = width2;
		heightMin = height1;
		heightMax = height2;
	}

	ChipHitCheck::ChipHitCheck() {
		_stageKey = "";
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
		auto stage = _stageChipsMap.find(_stageKey);	// ���݂̃X�e�[�W�̘A�z�z������o��
		auto chipdata = stage->second.find(no);	// �`�b�v�ԍ��̓����蔻����擾
		auto chip = chipdata->second;	// �o�^����Ă���ChipNumber���擾

		Vector2 min = { static_cast<double>(chip.WidthMin()), static_cast<double>(chip.HeightMin()) };
		Vector2 max = { static_cast<double>(chip.WidthMax()), static_cast<double>(chip.HeightMax()) };
		AABB chipbox = { min, max };
		return chipbox;
	}
}
