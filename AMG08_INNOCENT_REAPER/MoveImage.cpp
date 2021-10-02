#include "MoveImage.h"
#include "Game.h"

namespace {
	constexpr auto MOVE_COUNT = 60 * 40;
}

namespace inr {

	MoveImage::MoveImage(Game& game) : Image(game) {
		_start = {};
		_isMove = false;
	}

	void MoveImage::Init() {
		_pos = _start;
		_isMove = true;
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
		auto sizep = (end::STAFF_ROLL_HEIGHT - HALF_WINDOW_H);
		auto move_power = sizep / MOVE_COUNT;	// à⁄ìÆó ÇÃéZèo

		_pos.GetPY() -= move_power;

		auto max = -(end::STAFF_ROLL_HEIGHT / 2) + WINDOW_H;
		if (_pos.IntY() <= max) {
			// èCê≥ÇÇ©ÇØÇƒèàóùÇèIóπÇ∑ÇÈ
			_pos.GetPY() = -(end::STAFF_ROLL_HEIGHT / 2) + WINDOW_H;
			return true;
		}
		return false;
	}
}