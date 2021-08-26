#include "FadeBlack.h"

namespace {
	constexpr auto LUMINANCE = 255 / 60;
}

namespace inr {

	FadeBlack::FadeBlack(Game& game) : Image(game), _rgb() {
		_count = 0;
		_end = false;
		_calculation = 0;
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

	}

	void FadeBlack::Draw() {

	}
}