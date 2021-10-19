/*****************************************************************//**
 * @file   RedGreenBlue.cpp
 * @brief  RedGreenBlueカラーモデルクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "RedGreenBlue.h"

namespace inr {

	RedGreenBlue::RedGreenBlue() {
		_red = rgb::MIN_BLEND;
		_green = rgb::MIN_BLEND;
		_blue = rgb::MIN_BLEND;
	}

	RedGreenBlue::RedGreenBlue(double red, double green, double blue) {
		_red = red;
		_green = green;
		_blue = blue;

		SetBlend(&_red);
		SetBlend(&_green);
		SetBlend(&_blue);
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

	void RedGreenBlue::Min() {
		_red = rgb::MIN_BLEND;
		_green = rgb::MIN_BLEND;
		_blue = rgb::MIN_BLEND;
	}

	void RedGreenBlue::Max() {
		_red = rgb::MAX_BLEND;
		_green = rgb::MAX_BLEND;
		_blue = rgb::MAX_BLEND;
	}
}