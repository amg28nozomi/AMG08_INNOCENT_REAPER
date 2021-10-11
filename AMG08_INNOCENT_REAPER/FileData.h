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
	// ファイル情報
	class FileData {
	public:
		// コンストラクタ(引数1:パス　引数2:ファイル名　引数3:フォーマット)
		FileData(std::string path, std::string filename, std::string format);
		// デストラクタ
		~FileData() = default;
		// パスの取得
		inline std::string Path() { return _path; }
		// ファイル名の取得
		inline std::string FileName() { return _fileName; }
		// フォーマットの取得
		inline std::string Format() { return _format; }
		// 全情報取得
		inline std::string AllPath() { return _path + _fileName + _format; }
	private:
		std::string _path;			// パス
		std::string _fileName;		// ファイル名
		std::string _format;		// フォーマット
	};
}

