#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"

namespace inr{

	class ObjectServer {
	public:
		ObjectServer();
		~ObjectServer();

		void Clear();	// コンテナ初期化
		void Add(std::shared_ptr<ObjectBase> obj);
		void Del(std::shared_ptr<ObjectBase> obj);

		void Process();
		void Draw();

		ObjectBase& GetObjectA(ObjectBase::ObjectType otype); // 単品
		Player& GetPlayer();
		std::vector<std::shared_ptr<ObjectBase>>& GetObjects() { return _objects; }
		inline void ObjectsClear() { _objects.clear(); }
		// AABB GetObjectPosition(std::string key);
		// std::vector<std::unique_ptr<ObjectBase>>& GetObjectList(ObjectBase::ObjectType otype);
	private:

		bool _updateFlg;	// 更新があるか
		
		std::vector<std::shared_ptr<ObjectBase>> _objects;
		std::vector<std::shared_ptr<ObjectBase>> _addObj;
		std::vector<std::shared_ptr<ObjectBase>> _delObj;
	};
}

