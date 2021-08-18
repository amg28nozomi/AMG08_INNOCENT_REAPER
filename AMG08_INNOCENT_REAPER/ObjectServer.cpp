#include "ObjectServer.h"
#include "ObjectBase.h"
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
	}

	void ObjectServer::Add(std::unique_ptr<ObjectBase> obj) {
		// �X�V������ꍇ�́A�R���e�i�Ɉꎞ�I�Ɋi�[
		if (_updateFlg) {
			_addObj.emplace_back(std::move(obj));
		} else {
			// �Ȃ��ꍇ�͒��ڒǉ�
			_objects.emplace_back(std::move(obj));
		}
	}

	void ObjectServer::Process() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Process();
		}

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
	}

	void ObjectServer::Draw() {
		for (auto&& obj : _objects) {
			obj->Draw();
		}
		_updateFlg = false;
	}


	// �w�肵���I�u�W�F�N�g�����o���i�P�i�j
	ObjectBase& ObjectServer::GetObjectA(ObjectBase::ObjectType otype) {
		for (auto& it : _objects) {
			if (it->GetType() == otype) {
				return *it;
			}
		}
	}

	ObjectBase& ObjectServer::GetPlayer() {
		for (auto& it : _objects) {
			if (it->GetType() == ObjectBase::ObjectType::PLAYER) {
				return *it;
			}
		}
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