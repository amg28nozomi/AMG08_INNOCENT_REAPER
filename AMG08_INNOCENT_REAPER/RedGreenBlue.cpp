#include "RedGreenBlue.h"

namespace inr {

	RedGreenBlue::RedGreenBlue(double red, double green, double blue) {
		_red = red;
		_green = green;
		_blue = blue;
	}

	void RedGreenBlue::Update(bool type, double value) {
		switch (type) {
		case rgb::ADD:
			_red += value;
			_green += value;
			_blue += value;
			break;
		case rgb::SUB:
			_red -= value;
			_green -= value;
			_blue -= value;
			break;
		default:
			break;
		}

		// 値が上限・加減を超えた場合は修正する
		SetBlend(&_red);
		SetBlend(&_green);
		SetBlend(&_blue);
	}

	void RedGreenBlue::Update(bool type, double rvalue, double gvalue, double bvalue) {
		switch (type) {
		case rgb::ADD:
			_red += rvalue;
			_green += gvalue;
			_blue += bvalue;
			break;
		case rgb::SUB:
			_red -= rvalue;
			_green -= gvalue;
			_blue -= bvalue;
			break;
		default:
			break;
		}

		// 値が上限・加減を超えた場合は修正する
		SetBlend(&_red);
		SetBlend(&_green);
		SetBlend(&_blue);
	}

	void RedGreenBlue::SetBlend(double* value) {
		if (*value < rgb::MIN_BLEND) *value = rgb::MIN_BLEND;
		else if (rgb::MAX_BLEND < *value) *value = rgb::MAX_BLEND;
	}
}