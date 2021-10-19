/*****************************************************************//**
 * @file   FileData.h
 * @brief  ファイルデータクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace inr {
	/** ファイル情報 */
	class FileData {
	public:
		/**
		 * @brief								コンストラクタ
		 * @param path					パス
		 * @param filename			ファイル名
		 * @param format				フォーマット
		 */
		FileData(std::string path, std::string filename, std::string format);
		/**
		 * @brief								デストラクタ
		 */
		~FileData() = default;
		/**
		 * @brief								パスの取得
		 * @return							パスを返す
		 */
		inline std::string Path() { return _path; }
		/**
		 * @brief								ファイル名の取得
		 * @return							ファイル名を返す
		 */
		inline std::string FileName() { return _fileName; }
		/**
		 * @brief								フォーマットの取得
		 * @return							フォーマットを返す
		 */
		inline std::string Format() { return _format; }
		/**
		 * @brief								全情報の取得
		 * @return							全情報を返す
		 */
		inline std::string AllPath() { return _path + _fileName + _format; }
	private:
		std::string _path;			//!< パス
		std::string _fileName;	//!< ファイル名
		std::string _format;		//!< フォーマット
	};
}

