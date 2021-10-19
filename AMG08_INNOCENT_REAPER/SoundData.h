/*****************************************************************//**
 * @file   SoundData.h
 * @brief  サウンド情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace se{
	/** サウンドデータ */
	class SoundData {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param	filename	ファイル名
		 * @param	type			再生タイプ
		 */
		SoundData(std::string filename, int type);
		/**
		 * @brief						デストラクタ
		 */
		~SoundData() = default;
		/**
		 * @brief						ファイル名の取得
		 * @return					ファイル名を返す
		 */
		std::string& GetFill();
		/**
		 * @brief						サウンドハンドルの参照を取得
		 * @return					サウンドハンドルの参照を返す
		 */
		inline int& GetHandleP() { return _handle; }
		/**
		 * @brief						サウンドハンドルの取得
		 * @return					サウンドハンドルを返す
		 */
		inline int GetHandle() { return _handle; }
		/**
		 * @brief						再生タイプの取得
		 * @return					再生タイプを返す
		 */
		inline int GetPlayType() { return _playType; }
	private:
		std::string _filename;	//!< ファイル名/パス名
		int _handle;						//!< サウンドハンドル
		int _playType;					//!< 再生タイプ
	};
}
