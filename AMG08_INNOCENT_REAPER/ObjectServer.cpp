#include "ObjectServer.h"
#include "ObjectBase.h"
#include "EnemyBase.h"
#include "GimmickBase.h"
#include "Player.h"
#include <vector>

namespace inr {

	ObjectServer::ObjectServer() {
		_updateFlg = false;
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

	void ObjectServer::Del(std::shared_ptr<ObjectBase> obj) {
		if (_updateFlg) {
			_delObj.emplace_back(std::move(obj));
		} else {
			obj.reset();	// �t���O���Ȃ��ꍇ�͒��ڏ��L�����������
		}
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
			return;
		} else {
			// ����ꍇ��_objects�Ɉڂ�
			for (auto&& obj : _addObj) {
				_objects.emplace_back(std::move(obj));
			}
			_addObj.clear();
		}

		if (_delObj.empty()) return;
		else _delObj.clear();
	}

	void ObjectServer::Draw() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Draw();
		}
		_updateFlg = false;
	}

	std::shared_ptr<Player> ObjectServer::GetPlayer() {
		for (auto& it : _objects) {
			if (it->GetType() == ObjectBase::ObjectType::PLAYER) {
				return std::dynamic_pointer_cast<Player>(it);
			}
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

	std::vector<std::shared_ptr<GimmickBase>> ObjectServer::GetGimmicks() {
		std::vector<std::shared_ptr<GimmickBase>> gimmicks;
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::GIMMICK) continue;
			gimmicks.emplace_back(std::dynamic_pointer_cast<GimmickBase>(obj));
		}
		return gimmicks;
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