/*****************************************************************//**
 * @file   Vector2.cpp
 * @brief  2�����x�N�g���N���X
 * 
 * @author �A�~���[�Y�����g���f�B�A�����w�@
 * @date   October 2021
 *********************************************************************/
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