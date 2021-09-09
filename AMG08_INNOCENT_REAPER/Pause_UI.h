#pragma once
#include "Particle_Image.h"


namespace inr {

	class Pause_UI : Particle_Image {
	public:
		Pause_UI(Game& game);
		~Pause_UI() = default;

		void Process() override;
		void ChangePosition(bool type) override;
	private:
		int _no;	// Œ»İ‚¢‚éêŠ‚Í‚Ç‚±‚©
		
	};
}

