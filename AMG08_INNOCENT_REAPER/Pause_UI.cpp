#include "Pause_UI.h"

namespace inr {

	Pause_UI::Pause_UI(Game& game) : Particle_Image(game) {
		_imageType = image::particle::UI;
	}

	void Pause_UI::Process() {
		Particle_Image::Process();
	}

	void Pause_UI::ChangePosition(bool type) {
		switch (type) {
		case true:
			++_no;
			break;
		case false:
			--_no;
			break;
		}

		switch (_no) {
		case -1:
			_pos.GetPY() = 540;
			_no = 2;
			return;
		case 0:
			_pos.GetPY() = 300;
			return;
		case 1:
			_pos.GetPY() = 440;
			return;
		case 2:
			_pos.GetPY() = 540;
			return;
		case 3:
			_no = 0;
			_pos.GetPY() = 300;
			return;
		default:
			return;
		}
	}
}