#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include <DxLib.h>

namespace inr {

	BackGround::BackGround() {
		_position = { 1920 / 2, 1080 / 2 };
	}

	void BackGround::Draw() {
		auto x = _position.IntX();
		auto y = _position.IntY();

		auto gh = graph::ResourceServer::GetHandles(BACK_HH, 0);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}
}
