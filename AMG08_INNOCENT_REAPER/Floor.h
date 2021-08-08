#ifdef _DEBUG
#pragma once
#include "Vector2.h"

class Vector2;

// 仮の足場。
namespace inr {

	class Game;

	class Floor {
	public:
		Floor(Game& game);
		~Floor();
		void Init();	// 初期化
		void Process();	// 更新
		void Draw();	// 描画
	private:
		Game& _game;
		Vector2 _pos;	//座
	};

}

#endif