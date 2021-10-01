#include "MoveImage.h"
#include "Game.h"

namespace {
	constexpr auto MOVE_COUNT = 1500;
}

namespace inr {

	MoveImage::MoveImage(Game& game) : Image(game) {
		_start = {};
		_isMove = false;
	}

	void MoveImage::Init() {
		_pos = _start;
	}

	void MoveImage::Process() {
		if (_isMove != true) return;
		if (Move() == true) _isMove = false;
	}

	void MoveImage::Draw() {
		auto x = _pos.IntX();
		auto y = _pos.IntY();
		auto graph = graph::ResourceServer::GetHandles(_graphKey, 0);

		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
	}

	void MoveImage::SetParameter(const std::string gkey, Vector2 pos) {
		_graphKey = gkey;
		_start = pos;
		_pos = _start;
	}

	bool MoveImage::Move() {
		auto sizep = (_start.GetY() + HALF_WINDOW_H) * -1;
		auto move_power = sizep / MOVE_COUNT;	// à⁄ìÆó ÇÃéZèo

		_pos.GetPY() += move_power;
		if (HALF_WINDOW_H <= _pos.IntY()) {
			// èCê≥ÇÇ©ÇØÇƒèàóùÇèIóπÇ∑ÇÈ
			_pos.GetPY() = HALF_WINDOW_H;
			return true;
		}
		return false;
	}
}