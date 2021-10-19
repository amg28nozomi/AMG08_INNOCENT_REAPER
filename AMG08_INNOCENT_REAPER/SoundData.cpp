/*****************************************************************//**
 * @file   SoundData.cpp
 * @brief  ƒTƒEƒ“ƒhî•ñ
 *
 * @author —é–ØŠóŠC
 * @date   October 2021
 *********************************************************************/
#include "SoundData.h"

namespace se {

	SoundData::SoundData(std::string fillname, int type) : _filename(fillname), _handle(0), _playType(type) {

	}

	std::string& SoundData::GetFill() {
		std::string* fillname = &_filename;
		return *fillname;
	}
}
