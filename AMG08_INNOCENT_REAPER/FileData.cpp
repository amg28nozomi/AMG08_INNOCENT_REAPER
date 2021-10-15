/*****************************************************************//**
 * @file   FileData.cpp
 * @brief  ファイルデータクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "FileData.h"

namespace inr {
	// コンストラクタ
	FileData::FileData(std::string path, std::string filename, std::string format) : _path(path), _fileName(filename), _format(format) {
	}
}