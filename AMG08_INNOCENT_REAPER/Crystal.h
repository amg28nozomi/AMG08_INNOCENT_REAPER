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
		std::shared_ptr<SoulSkin> _soul;	// •Û‚µ‚Ä‚¢‚é°
		std::vector<std::shared_ptr<Door>> _doors;	// •Û‚µ‚Ä‚¢‚éƒhƒA

		void SetDoors(std::string key);
		void GraphKey();
		void AddSoul();	// °‚Ì¶¬

		
	};
}

