#pragma once
#include "Image.h"
#include "Collision.h"
#include "RedGreenBlue.h"

namespace inr {
	class TitleLogo : public Image {
	public:
		TitleLogo(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		Vector2 _position2;
		Collision _hitCol1;	// “–‚½‚è”»’è
		Collision _hitCol2;

		int _pal;	// 
		std::string _graphKey2;	// ‚à‚¤ˆê‚Â‚ÌƒOƒ‰ƒnƒ“

		void DebugBox(Collision col);
		void CollisionOut();
	};
}

