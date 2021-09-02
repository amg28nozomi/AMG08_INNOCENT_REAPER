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
		// 更新がある場合は、コンテナに一時的に格納
		if (_updateFlg) {
			_addObj.emplace_back(std::move(obj));
		} else {
			// ない場合は直接追加
			_objects.emplace_back(std::move(obj));
		}
	}

	void ObjectServer::Del(std::shared_ptr<ObjectBase> obj) {
		if (_updateFlg) {
			_delObj.emplace_back(std::move(obj));
		} else {
			obj.reset();	// フラグがない場合は直接所有権を放棄する
		}
	}

	void ObjectServer::Process() {
		_updateFlg = true;
		for (auto&& obj : _objects) {
			obj->Process();
		}
		_updateFlg = false;

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