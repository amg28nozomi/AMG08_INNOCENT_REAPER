/*****************************************************************//**
 * @file   MapDataManager.cpp
 * @brief  �}�b�v���̊Ǘ����s���}�b�v�f�[�^�}�l�[�W��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "MapDataManager.h"
#include "MapData.h"

namespace inr {
	// �R���X�g���N�^
	MapDataManager::MapDataManager(Game& game) : _game(game) {
		StageMapClear();	// ������
	}
	// �R���X�g���N�^
	MapDataManager::~MapDataManager() {
		StageMapClear();	// �R���e�i�̉��
	}
	// �}�b�v���̈ꊇ�ǂݍ���
	void MapDataManager::LoadStageMap(JsonMapData& jsonMapData) {
		// �L�[�ƒl�̎��o��
		for (auto&& j : jsonMapData) {
			auto it = _maps.find(j.first);		// �L�[�͓o�^����Ă��邩
			if (it != _maps.end()) continue;	// ���ɓo�^����Ă���ꍇ�͏������X�L�b�v
			_maps.emplace(j.first, j.second);	// �o�^
		}
	}
	// �w�肵���}�b�v���̓ǂݍ���
	bool MapDataManager::GetStageMap(const std::string stage, MapData& mdata) {
		auto smap = _maps.find(stage);					// �L�[�͓o�^����Ă��邩
		if (smap == _maps.end()) return false;	// �w�肵���}�b�v���͑��݂��Ȃ�
		mdata = smap->second;										// ���o��������������
		return true;														// �ǂݍ��ݐ���
	}
	// �R���e�i�̉��
	void MapDataManager::StageMapClear() {
		_maps.clear();
	}
	// �ǂݍ��ݏ������s�����̔���
	bool MapDataManager::IsLoad(const std::string key) {
		auto it = _maps.find(key);					// �L�[�͓o�^����Ă��邩
		if (it == _maps.end()) return true;	// �o�^����Ă��Ȃ�
		return false;												// �o�^����Ă���
	}
}
