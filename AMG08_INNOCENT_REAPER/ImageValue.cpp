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
}