#pragma once
#include "Image.h"

namespace inr {

	class Particle_Image : public Image {
	public:
		Particle_Image(Game& game);
		~Particle_Image() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(Vector2 pos, std::string graph, int pal, double extrate);
	private:
		int _pal;	// “§–¾’l
		double _extRate;	// Šg‘å—¦
	};
}

