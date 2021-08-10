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

		void Clear();	// �R���e�i������
		void Add(std::unique_ptr<ObjectBase> obj);

		void Process();
		void Draw();

		ObjectBase& GetObjectA(ObjectBase::ObjectType otype); // �P�i
		ObjectBase& GetPlayer();
		std::vector<std::unique_ptr<ObjectBase>>& GetObjects() { return _objects; }
		// AABB GetObjectPosition(std::string key);
		// std::vector<std::unique_ptr<ObjectBase>>& GetObjectList(ObjectBase::ObjectType otype);
	private:

		bool _updateFlg;	// �X�V�����邩
		
		std::vector<std::unique_ptr<ObjectBase>> _objects;
		std::vector<std::unique_ptr<ObjectBase>> _addObj;
	};
}

