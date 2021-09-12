#pragma once
#include "GimmickBase.h"
#include "SoulSkin.h"
#include <memory>
#include <vector>

namespace inr {

	class Door;

	class Crystal : public GimmickBase {
	public:
		Crystal(Game& game);
		~Crystal() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;
		void ObjValueUpdate() override;
		void CollisionHit(const std::string ckey, Collision acollision, bool direction);

		// int HaveSoul();
	private:
		std::shared_ptr<SoulSkin> _soul;	// 保持している魂
		std::vector<std::shared_ptr<Door>> _doors;	// 保持しているドア

		void SetDoors(std::vector<std::string> key);
		void GraphKey();
		void AddSoul();	// 魂の生成

		bool IsOpen(int crystal);	// ドアを開けるか？

		
	};
}

