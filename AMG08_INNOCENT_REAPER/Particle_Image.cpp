#include "Particle_Image.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace inr {

	Particle_Image::Particle_Image(Game& game) : Image(game) {

	}

	void Particle_Image::Init() {
		_pal = 0;
	}

	void Particle_Image::Process() {

	}

	void Particle_Image::Draw() {
		auto x = _pos.IntX();
		auto y = _pos.IntY();
		auto graph = graph::ResourceServer::GetHandles(_graphKey, 0);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, _pal);
		DrawRotaGraph(x, y, _extRate, 0, graph, TRUE, FALSE);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}

	void Particle_Image::SetParameter(Vector2 pos, std::string graph, int pal, double extrate) {
		_pos = pos;
		_graphKey = graph;
		_pal = pal;
		_extRate = extrate;
	}
}
