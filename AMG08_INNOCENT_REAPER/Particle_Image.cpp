#include "Particle_Image.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace {
	constexpr auto PAL_VALUE = 5;
}

namespace inr {

	Particle_Image::Particle_Image(Game& game) : Image(game) {
		Init();
	}

	void Particle_Image::Init() {
		_animation = animation::A_ADD;
		_pal = 0;
		_extRate = 1.0;
		_isDraw = false;
	}

	void Particle_Image::Process() {
		if (_isDraw != true) return;
		Animation();
	}

	void Particle_Image::Draw() {
		if (_isDraw != true) return;
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

	bool Particle_Image::Animation() {
		// 真の場合のみノーマル状態に移行
		switch (_animation) {
		case animation::A_NORMAL:
			return false;
		case animation::A_ADD:
			if (AddPal() != true) return false;
			break;
		case animation::A_SUB:
			if (SubPal() != true) return false;
			break;
		default:
			return false;
		}
		_animation = animation::A_NORMAL;
		return true;
	}

	bool Particle_Image::DrawStart() {
		if (_isDraw == true) return false;
		_isDraw = true;
		_animation = animation::A_ADD;
	}

	bool Particle_Image::DrawEnd() {
		if (_isDraw != true && _animation != animation::A_SUB) return false;	// 描画処理がある場合のみ
		_animation = animation::A_SUB;	// 
	}

	bool Particle_Image::AddPal() {
		_pal += PAL_VALUE;
		if (_pal < 255) return false;
		if (255 < _pal) _pal = 255;
		return true;
	}

	bool Particle_Image::SubPal() {
		_pal -= PAL_VALUE;
		if (0 < _pal) return false;
		if (_pal < 0) _pal = 255;
		return true;
	}
}
