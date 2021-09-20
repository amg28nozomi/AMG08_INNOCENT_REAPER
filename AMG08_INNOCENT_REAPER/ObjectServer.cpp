#include "ObjectServer.h"
#include "ObjectBase.h"
#include "EnemyBase.h"
#include "GimmickBase.h"
#include "SoulSkin.h"
#include "Player.h"
#include "CrowDoll.h"
#include "ObjectValue.h"
#include <vector>

namespace inr {

	ObjectServer::ObjectServer() {
		_updateFlg = false;
		_delete = false;
		Clear();
	}

	ObjectServer::~ObjectServer() {
		Clear();
	}

	void ObjectServer::Clear() {
		_objects.clear();
		_addObj.clear();
		_delObj.clear();
	}

	void ObjectServer::Add(std::shared_ptr<ObjectBase> obj) {
		// �X�V������ꍇ�́A�R���e�i�Ɉꎞ�I�Ɋi�[
		if (_updateFlg) {
			_addObj.emplace_back(std::move(obj));
		} else {
			// �Ȃ��ꍇ�͒��ڒǉ�
			_objects.emplace_back(std::move(obj));
		}
	}

	void ObjectServer::DeleteObject() {
		auto osize = static_cast<int>(_objects.size());
		auto fix = 0;
		for (int number = 0; number < osize; ++number) {
			// �����O�Ə������ŃT�C�Y���Ⴄ�ꍇ�͏C�����s��
			if (static_cast<int>(_objects.size()) < osize) {
				++fix;
			}
			// �Ώۂ��������邩�H
			if (_objects.at(number - fix)->IsDelete() == false) continue;
			_delObj.emplace_back(std::move(_objects.at(number - fix)));
			_objects.erase(_objects.begin() + (number - fix));
		}

		_delObj.clear();	// �I�u�W�F�N�g���J��
		_delete = false;	// �����t���O���I�t�ɂ���
	}

	void ObjectServer::Process() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Process();
		}
		_updateFlg = false;

		// �v�f�����邩�ǂ���
		if (_addObj.empty()) {
			// �Ȃ��ꍇ�͒E�o
		} else {
			// ����ꍇ��_objects�Ɉڂ�
			for (auto&& obj : _addObj) {
				_objects.emplace_back(std::move(obj));
			}
			_addObj.clear();
		}

		if (_delete == true) DeleteObject();
	}

	void ObjectServer::Draw() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Draw();
		}
		_updateFlg = false;
	}

	void ObjectServer::ObjectsClear() {
		// �v���C���[�ȊO���T�[�o�[����폜
		int fix = 0;	// �C���l
		std::shared_ptr<ObjectBase> player = nullptr;
		std::vector<ObjectValue> _gimmickValues;	// �M�~�b�N�̏��X�V�p
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::PLAYER) continue;
			player = obj;	// ���@�̂ݕێ�
		}
		_objects.clear();	// �z�񏉊���
		if(player != nullptr)_objects.emplace_back(std::move(player));	// ���@�̃A�h���X������ꍇ�̂ݓo�^
	}

	bool ObjectServer::IsPlayer() {
		if (_objects.empty()) return false;	// ���Ȃ�
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::PLAYER) continue;
			return true;
		}
		return false;
	}

	std::shared_ptr<Player> ObjectServer::GetPlayer() {
		for (auto& it : _objects) {
			if (it->GetType() == ObjectBase::ObjectType::PLAYER) {
				return std::dynamic_pointer_cast<Player>(it);
			}
		}
	}

	std::shared_ptr<CrowDoll> ObjectServer::GetBoss() {
		auto enemys = GetEnemys();
		for (auto& it : enemys) {
			if (it->IsBoss() != true) continue;
			return std::dynamic_pointer_cast<CrowDoll>(it);
		}
	}

	std::vector<std::shared_ptr<EnemyBase>> ObjectServer::GetEnemys() {
		std::vector<std::shared_ptr<EnemyBase>> enemys;
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::ENEMY) continue;
			enemys.emplace_back(std::dynamic_pointer_cast<EnemyBase>(obj));
		}
		return enemys;
	}

	/*std::vector<std::shared_ptr<GimmickBase>> ObjectServer::GetGimmicks() {
		std::vector<std::shared_ptr<GimmickBase>> gimmicks;
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::GIMMICK) continue;
			gimmicks.emplace_back(std::dynamic_pointer_cast<GimmickBase>(obj));
		}
		return gimmicks;
	}*/

	std::shared_ptr<SoulSkin> ObjectServer::GetSoul() {
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::SOUL) continue;
			auto osoul = std::dynamic_pointer_cast<SoulSkin>(obj);
			if (osoul->IsGive() == false) continue;
			return osoul;
		}
	}

	void ObjectServer::GimmickUpdate() {

	}


	// �w�肵���I�u�W�F�N�g�̎Q�Ƃ����o���i�z��j
	/*std::vector<std::unique_ptr<ObjectBase>>& ObjectServer::GetObjectList(ObjectBase::ObjectType otype) {
		std::vector<std::unique_ptr<ObjectBase>>& list = _objects;

		for (auto& it : list) {
			if (it->GetType() == otype) {
				list.emplace_back(&it);
			}
		}
		return list;
	}*/
}