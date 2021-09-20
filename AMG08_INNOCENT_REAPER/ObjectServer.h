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

		void Clear();	// �R���e�i������
		void Add(std::shared_ptr<ObjectBase> obj);

		void Process();
		void Draw();

		void DeleteObject();	// �I�u�W�F�N�g�̊J���������s��

		std::shared_ptr<Player> GetPlayer();
		std::vector<std::shared_ptr<ObjectBase>> GetObjects() { return _objects; }

		std::vector<std::shared_ptr<EnemyBase>> GetEnemys();
		/*std::vector<std::shared_ptr<GimmickBase>> GetGimmicks();*/
		std::shared_ptr<SoulSkin> GetSoul();
		std::shared_ptr<CrowDoll> GetBoss();


		bool IsPlayer();	// �v���C���[�͐�������Ă��邩�H
		void ObjectsClear();
		inline void AllClear() { _objects.clear(); }
		inline void DelOn() { _delete = true; }
		inline bool DelFlag() { return _delete; }
		// AABB GetObjectPosition(std::string key);
		// std::vector<std::unique_ptr<ObjectBase>>& GetObjectList(ObjectBase::ObjectType otype);
	private:

		bool _updateFlg;	// �X�V�����邩
		bool _delete;
		
		std::vector<std::shared_ptr<ObjectBase>> _objects;
		std::vector<std::shared_ptr<ObjectBase>> _addObj;
		std::vector<std::shared_ptr<ObjectBase>> _delObj;

		void GimmickUpdate();	// �M�~�b�N�ɏC���������� 
	};
}

