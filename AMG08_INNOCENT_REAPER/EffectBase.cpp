#include "EffectBase.h"
#include "MapChips.h"
#include <DxLib.h>

namespace inr {

	EffectBase::EffectBase(Game& game, const Vector2 spawnpos) : _game(game), _position(spawnpos){
		// 
		_count = std::make_pair(effect::NO_MAXFRAME, effect::NO_COUNT);

	}

	EffectBase::~EffectBase() {

	}

	void EffectBase::Init() {

	}

	void EffectBase::Process() {

	}

	void EffectBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
	}
}