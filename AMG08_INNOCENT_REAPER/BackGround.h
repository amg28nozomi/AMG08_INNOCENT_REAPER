#pragma once
#include "Vector2.h"

namespace inr {
	class Game;

	class BackGround {
	public:
		BackGround(Game& game);
		
		void Process();
		void Draw();
	private:
		Vector2 _position1;	// ï`âÊç¿ïW
		Vector2 _position2;

		Game& _game;
	};
}

