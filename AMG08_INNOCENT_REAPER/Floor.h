#ifdef _DEBUG
#pragma once
#include "Vector2.h"

class Vector2;

// ‰¼‚Ì‘«êB
namespace inr {

	class Game;

	class Floor {
	public:
		Floor(Game& game);
		~Floor();
		void Init();	// ‰Šú‰»
		void Process();	// XV
		void Draw();	// •`‰æ
	private:
		Game& _game;
		Vector2 _pos;	//À
	};

}

#endif