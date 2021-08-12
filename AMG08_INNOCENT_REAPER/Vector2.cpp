#include "Vector2.h"

void Vector2::Normalize() {
	auto l = Length();
	if (-0.1 < l < 0.1) {
		x = 0.0;
		y = 0.0;
	} else {
		x /= l;
		y /= l;
	}
}