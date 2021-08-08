#pragma once
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr{
	class ObjectServer {
	public:
		ObjectServer();
		~ObjectServer();

		void Clear();	// コンテナ初期化
		void Add(std::unique_ptr<ObjectBase> obj);
		void Process();
		void Draw();

		ObjectBase& GetObjectA(ObjectBase::ObjectType otype); // 
		// AABB GetObjectPosition(std::string key);
		// std::vector<ObjectBase&> GetObjectList(ObjectBase::ObjectType otype);
	private:
		bool _updateFlg;	// 更新があるか
		std::vector<std::unique_ptr<ObjectBase>> _objects;
		std::vector<std::unique_ptr<ObjectBase>> _addObj;
	};
}

