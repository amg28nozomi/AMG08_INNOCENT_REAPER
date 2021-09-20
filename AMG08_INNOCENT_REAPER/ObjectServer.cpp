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
		// 更新がある場合は、コンテナに一時的に格納
		if (_updateFlg) {
			_addObj.emplace_back(std::move(obj));
		} else {
			// ない場合は直接追加
			_objects.emplace_back(std::move(obj));
		}
	}

	void ObjectServer::DeleteObject() {
		auto osize = static_cast<int>(_objects.size());
		auto fix = 0;
		for (int number = 0; number < osize; ++number) {
			// 処理前と処理中でサイズが違う場合は修正を行う
			if (static_cast<int>(_objects.size()) < osize) {
				++fix;
			}
			// 対象を消去するか？
			if (_objects.at(number - fix)->IsDelete() == false) continue;
			_delObj.emplace_back(std::move(_objects.at(number - fix)));
			_objects.erase(_objects.begin() + (number - fix));
		}

		_delObj.clear();	// オブジェクトを開放
		_delete = false;	// 消去フラグをオフにする
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
		} else {
			// ある場合は_objectsに移す
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
		// プレイヤー以外をサーバーから削除
		int fix = 0;	// 修正値
		std::shared_ptr<ObjectBase> player = nullptr;
		std::vector<ObjectValue> _gimmickValues;	// ギミックの情報更新用
		for (auto obj : _objects) {
			if (obj->GetType() != ObjectBase::ObjectType::PLAYER) continue;
			player = obj;	// 自機のみ保持
		}
		_objects.clear();	// 配列初期化
		if(player != nullptr)_objects.emplace_back(std::move(player));	// 自機のアドレスがある場合のみ登録
	}

	bool ObjectServer::IsPlayer() {
		if (_objects.empty()) return false;	// 居ない
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