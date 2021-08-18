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
		// 更新がある場合は、コンテナに一時的に格納
		if (_updateFlg) {
			_addObj.emplace_back(std::move(obj));
		} else {
			// ない場合は直接追加
			_objects.emplace_back(std::move(obj));
		}
	}

	void ObjectServer::Process() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Process();
		}

		// 要素があるかどうか
		if (_addObj.empty()) {
			// ない場合は脱出
			return;
		} else {
			// ある場合は_objectsに移す
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


	// 指定したオブジェクトを取り出す（単品）
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

	// 指定したオブジェクトの参照を取り出す（配列）
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