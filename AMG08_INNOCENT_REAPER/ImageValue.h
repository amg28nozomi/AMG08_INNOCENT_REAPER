#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	class ParticleValue {
	public:
		ParticleValue();
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		~ParticleValue() = default;

		inline bool Animation() { return _animation; }
		inline bool Particle() { return _particle; }
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	// �A�j���[�V�����̍Đ����s����
		bool _particle;	// ���ߏ������s����
		double _exrate;	// �g�嗦�i�f�t�H���g����1.0�{�j
	};

	class ImageValue {
	public:
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		~ImageValue() = default;

		inline std::string GraphKey() { return _graph; }
		inline Vector2 Position() { return _position; }
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;	// �摜�L�[
		Vector2 _position;	// ���W
		ParticleValue _particleValue;
	};
}

