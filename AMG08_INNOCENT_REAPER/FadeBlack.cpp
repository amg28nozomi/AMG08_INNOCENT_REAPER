#include "FadeBlack.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace {
	constexpr auto LUMINANCE = 255 / 60;

	constexpr auto FADE_VALUE = 255 / 60;
}

namespace inr {

	FadeBlack::FadeBlack(Game& game) : Image(game) {
		_pal = 0;
		_type = image::FADE_OUT;
		Init();
	}

	FadeBlack::~FadeBlack() {

	}

	void FadeBlack::Init() {
		_end = true;
		_interval = 0;
		_isInterval = false;
		_addEnd = true;
	}

	void FadeBlack::Process() {
		if (_end == true) return;	// 処理がない場合はスキップ
		if (_addEnd == true) _addEnd = false;
		if (_isInterval) {	// 猶予時間がある場合はカウンタを減らす
			if (_interval == 0) {
				_isInterval = false;
				_type = image::FADE_IN;
				return;
			}
			--_interval;
			return;
		}

		// 処理がある場合は回す
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
		if (_end == true) return;
		auto graph = graph::ResourceServer::GetHandles(image::BLACK, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(961, 540, 1.0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void FadeBlack::FlagChange(bool fadeType, int interval) { 
		_type = fadeType;
		_end = false;
		_interval = interval;
	}

	void FadeBlack::FadeIn() {
		_pal -= FADE_VALUE;
		FadeEnd();
	}

	void FadeBlack::FadeOut() {
		_pal += FADE_VALUE;
		FadeEnd();
	}

	bool FadeBlack::FadeEnd() {
		switch (_type) {
		case image::FADE_IN:
			if (0 < _pal) return false;
			if (_pal < 0) _pal = 0;
			_end = true;
			_isInterval = false;
			return true;
		case image::FADE_OUT:
			if (_pal < 255) return false;
			if (255 < _pal) _pal = 255;
			_addEnd = true;
			break;
		}
		_isInterval = true;
		return true;
	}
}