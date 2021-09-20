#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"

namespace inr{

	namespace object {
		constexpr auto CONTINUE = false;
		constexpr auto TRANSITION = true;
	}

	class EnemyBase;
	class CrowDoll;
	class GimmickBase;

	class ObjectServer {
	public:
		ObjectServer();
		~ObjectServer();

		void Clear();	// コンテナ初期化
		void Add(std::shared_ptr<ObjectBase> obj);

		void Process();
		void Draw();

		void DeleteObject();	// オブジェクトの開放処理を行う

		std::shared_ptr<Player> GetPlayer();
		std::vector<std::shared_ptr<ObjectBase>> GetObjects() { return _objects; }

		std::vector<std::shared_ptr<EnemyBase>> GetEnemys();
		/*std::vector<std::shared_ptr<GimmickBase>> GetGimmicks();*/
		std::shared_ptr<SoulSkin> GetSoul();
		std::shared_ptr<CrowDoll> GetBoss();


		bool IsPlayer();	// プレイヤーは生成されているか？
		void ObjectsClear();
		inline void AllClear() { _objects.clear(); }
		inline void DelOn() { _delete = true; }
		inline bool DelFlag() { return _delete; }
		// AABB GetObjectPosition(std::string key);
		// std::vector<std::unique_ptr<ObjectBase>>& GetObjectList(ObjectBase::ObjectType otype);
	private:

		bool _updateFlg;	// 更新があるか
		bool _delete;
		
		std::vector<std::shared_ptr<ObjectBase>> _objects;
		std::vector<std::shared_ptr<ObjectBase>> _addObj;
		std::vector<std::shared_ptr<ObjectBase>> _delObj;

		void GimmickUpdate();	// ギミックに修正をかける 
	};
}

