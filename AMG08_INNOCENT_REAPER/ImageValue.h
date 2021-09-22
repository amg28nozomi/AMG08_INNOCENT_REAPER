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
		bool _animation;	// アニメーションの再生を行うか
		bool _particle;	// 透過処理を行うか
		double _exrate;	// 拡大率（デフォルトだと1.0倍）
	};

	class ImageValue {
	public:
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		~ImageValue() = default;

		inline std::string GraphKey() { return _graph; }
		inline Vector2 Position() { return _position; }
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;	// 画像キー
		Vector2 _position;	// 座標
		ParticleValue _particleValue;
	};
}

