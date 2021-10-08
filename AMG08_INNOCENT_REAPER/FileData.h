/*****************************************************************//**
 * \file   FileData.h
 * \brief  ファイルデータクラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace inr {
	class FileData {
	public:
		FileData(std::string path, std::string filename, std::string format);
		~FileData() = default;

		inline std::string Path() { return _path; }
		inline std::string FileName() { return _fileName; }
		inline std::string Format() { return _format; }
		inline std::string AllPath() { return _path + _fileName + _format; }
	private:
		std::string _path;
		std::string _fileName;
		std::string _format;
	};
}

