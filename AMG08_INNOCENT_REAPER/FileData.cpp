/*****************************************************************//**
 * @file   FileData.cpp
 * @brief  ファイルデータクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "FileData.h"

namespace inr {

  FileData::FileData(std::string path, std::string fileName, std::string format) : _path(path), _fileName(fileName), _format(format) {
  }
}