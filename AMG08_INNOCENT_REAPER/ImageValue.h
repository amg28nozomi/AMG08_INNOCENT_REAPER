#pragma once
#include "Vector2.h"
#include <string>
#include <vector>

namespace inr {

	namespace pvalue {
		constexpr auto ITEM_0 = 0;
		constexpr auto ITEM_1 = 1;
		constexpr auto ITEM_2 = 2;
		constexpr auto ITEM_3 = 3;
	}

	namespace item {
		constexpr auto MESSAGE_0 = "message_0";
		constexpr auto MESSAGE_1 = "message_1";
		constexpr auto MESSAGE_2 = "message_2";
		constexpr auto MESSAGE_3 = "message_3";
	}

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
		ImageValue();
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

	class TutorialValue {
	public:
		TutorialValue();
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);

		inline ImageValue GetImageValue() { return _ivalue; }
		std::vector<int> Width() { return _width; }
		std::vector<int> Height() { return _height; }

	private:
		ImageValue _ivalue;
		std::vector<int> _width;
		std::vector<int> _height;
	};
}

