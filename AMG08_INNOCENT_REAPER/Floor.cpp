#ifdef _DEBUG
#include "Floor.h"
#include "Collision.h"
#include "Vector2.h"
#include "Game.h"


namespace inr {

	Floor::Floor(Game& game) : _game(game){

	}

	Floor::~Floor() {
		
	}

	void Floor::Init() {
		auto minx = DEBUG_WINDOW_W;
		auto miny = DEBUG_WINDOW_H;
	}

	void Floor::Process() {
		// 当たり判定のを呼び出す

	}

	void Floor::Draw() {

	}
}
#endif