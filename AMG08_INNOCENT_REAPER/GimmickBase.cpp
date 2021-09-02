#include "GimmickBase.h"
#include "Game.h"

namespace inr {

	GimmickBase::GimmickBase(Game& game) : ObjectBase(game), _gimmick(GimmickType::LEVER) {

	}

	GimmickBase::~GimmickBase() {

	}

	void GimmickBase::Init() {

	}

	void GimmickBase::Process() {

	}

	void GimmickBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
	}
}