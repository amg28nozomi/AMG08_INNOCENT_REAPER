/*****************************************************************//**
 * @file   SoundData.cpp
 * @brief  �T�E���h���
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "SoundData.h"

namespace se {

	SoundData::SoundData(std::string filename, int type) : _filename(filename), _handle(0), _playType(type) {

	}

	std::string& SoundData::GetFill() {
		std::string* fillname = &_filename;
		return *fillname;
	}
}
