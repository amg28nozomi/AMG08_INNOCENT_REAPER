#include "ImageValue.h"

namespace {
	constexpr auto DEFAULT_RATE = 1.0;
}

namespace inr {

	ImageValue::ImageValue() : _particleValue(), _position() {
		_graph = "";
	}

	ImageValue::ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue) : _particleValue(paricleValue), _position(pos) {
		_graph = gkey;
	}

	ParticleValue::ParticleValue() {
		_animation = false;
		_particle = false;
		_exrate = DEFAULT_RATE;
	}

	ParticleValue::ParticleValue(bool animation, bool particle, double exrate) {
		_animation = animation;
		_particle = particle;
		_exrate = exrate;
	}

	TutorialValue::TutorialValue() : _ivalue() {
		_width.emplace_back(0);
		_height.emplace_back(0);
	}

	TutorialValue::TutorialValue(ImageValue ivalue, int width, int height) : _ivalue(ivalue) {
		_width.emplace_back(width);
		_height.emplace_back(height);
	}

	TutorialValue::TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2) : _ivalue(ivalue) {
		_width = { width1, width2 };
		_height = { height1, height2 };
	}
}