/*****************************************************************//**
 * @file   SoundData.cpp
 * @brief  �T�E���h���
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "SoundData.h"

namespace se {

  SoundData::SoundData(std::string fileName, int type) : _filename(fileName), _handle(0), _playType(type) {
  }

  std::string& SoundData::GetFill() {
    std::string* fileName = &_filename;
    return *fileName;
  }
}