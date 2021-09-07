#include "FadeBlack.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace {
	constexpr auto LUMINANCE = 255 / 60;

	constexpr auto FADE_VALUE = 255 / 60;
}

namespace inr {

	FadeBlack::FadeBlack(Game& game) : Image(game) {
		_pal = 255;
		_count = 0;
		_end = false;
		_calculation = 0;
		_type = image::FADE_OUT;
		Init();
	}

	FadeBlack::~FadeBlack() {

	}

	void FadeBlack::Init() {
		_count = 0;
		_end = false;
		_calculation = 0;

	}

	void FadeBlack::Process() {
		if (_end == true) return;	// 処理がない場合はスキップ
		switch (_type) {
		case image::FADE_IN:
			FadeIn();
			return;
		case image::FADE_OUT:
			FadeOut();
			return;
		}
	}

	void FadeBlack::Draw() {
		auto graph = graph::ResourceServer::GetHandles(image::BLACK, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(0, 0, 1.0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void FadeBlack::FadeIn() {
		_pal += FADE_VALUE;
		FadeEnd();
	}

	void FadeBlack::FadeOut() {
		_pal += FADE_VALUE;
		FadeEnd();
	}

	bool FadeBlack::FadeEnd() {
		if (0 < _pal && _pal < 255) return false;
		switch (_type) {
		case image::FADE_IN:
			if (_pal < 0) _pal = 0;
			break;
		case image::FADE_OUT:
			if (255 < _pal) _pal = 255;
			break;
		}
		_end = true;
		return true;
	}
}