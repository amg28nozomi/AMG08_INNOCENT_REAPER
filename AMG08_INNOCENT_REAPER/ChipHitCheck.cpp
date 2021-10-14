/*****************************************************************//**
 * @file   ChipHitCheck.cpp
 * @brief  �}�b�v�`�b�v�̓o�^�����Ǘ�����`�b�v�q�b�g�`�F�b�N�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ChipHitCheck.h"
#include "Collision.h"
#include "Vector2.h"
#include "ChipNumber.h"

namespace inr {
	/** �R���X�g���N�^ */
	// �R���X�g���N�^
	ChipHitCheck::ChipHitCheck() {
		// �e�평����
		_chipKey = "";
		ClearStageMaps();
	}
	// �f�X�g���N�^
	ChipHitCheck::~ChipHitCheck() {
		ClearStageMaps();
	}
	// �o�^���̏�����
	void ChipHitCheck::ClearStageMaps() {
		for (auto&& scm : _stageChipsMap) {
			auto& key = scm.first;	// �Q�ƌ��̃L�[�Ō���
			auto chip = _stageChipsMap.find(key);
			chip->second.clear();	// �A�z�z�񏉊���
		}
		_stageChipsMap.clear();		// �R���e�i�̉��
	}
	// �}�b�v�`�b�v�̓����蔻��o�^
	void ChipHitCheck::LoadChipsMap(std::string key, ChipsMap& chipsMap) {
		// ���̏��͊��ɓo�^����Ă��邩�H
		auto it = _stageChipsMap.find(key);
		// �q�b�g�����ꍇ�͏������甲����
		if (it != _stageChipsMap.end()) return;
		_stageChipsMap.emplace(key, chipsMap);	// �A�z�z��ɓo�^
	}
	// �ΏۂɌ��ʂ����邩�̔���(����:������s���`�b�v�ԍ�)
	int ChipHitCheck::IsChipType(const int no) {
		auto stage = _stageChipsMap.find(_chipKey);	// ���݂̃X�e�[�W�̘A�z�z������o��
		auto chipnumber = stage->second.find(no);	// �`�b�v�ԍ��̓����蔻����擾
		if (chipnumber == stage->second.end()) return mapchip::NORMAL;	// �Y���Ȃ�
		// ���ʂ͂��邩�H
		switch (chipnumber->second.ChipType()) {
		case mapchip::NORMAL:
			return mapchip::NORMAL;		// ���ʂȂ�
		case mapchip::THORM:
			return mapchip::THORM;		// �_���[�W����
		case mapchip::IVY:
			return mapchip::IVY;		// �Ӕ���
		case mapchip::TRANSITION:
			return mapchip::TRANSITION;	// �X�e�[�W�J�ڔ���
		default:
			return mapchip::NONE;		// �o�^����Ă��Ȃ��`�b�v�ԍ�
		}
	}
	// �Ώۂɓ����蔻�肪���邩�̔���(����:������s���`�b�v�ԍ�)
	bool ChipHitCheck::IsHitType(const int no) {
		auto stage = _stageChipsMap.find(_chipKey);	// ���݂̃X�e�[�W�̘A�z�z������o��
		auto chipnumber = stage->second.find(no);	// �`�b�v�ԍ��̓����蔻����擾
		if (chipnumber == stage->second.end()) return true;
		// �����蔻��͂��邩�H
		switch (chipnumber->second.HitType()) {
		case mapchip::HIT_ON:	// �����蔻��L��
			return true;
		case mapchip::HIT_OFF:	// �����蔻�薳��
			return false;
		}
	}
	// �Ώۂ̓����蔻����擾(����:�擾���s���`�b�v�ԍ�)
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
			auto chip = chipdata->second;	// �o�^����Ă���ChipNumber����
			min = { static_cast<double>(chip.WidthMin()), static_cast<double>(chip.HeightMin()) };
			max = { static_cast<double>(chip.WidthMax()), static_cast<double>(chip.HeightMax()) };
		}
		AABB chipbox = { min, max };
		return chipbox;
	}
}
