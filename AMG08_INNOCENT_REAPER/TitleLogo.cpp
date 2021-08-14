#include "TitleLogo.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace inr {

	TitleLogo::TitleLogo(Game& game) : Image(game) {
		Init();
	}

	void TitleLogo::Init() {
		_pos = { 960,540 };
		_graphKey = TITLE_LOGO;
	}

	void TitleLogo::Draw() {
		auto x = _pos.IntX();
		auto y = _pos.IntY();

		auto gh = graph::ResourceServer::GetHandles(_graphKey, 0);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}
}