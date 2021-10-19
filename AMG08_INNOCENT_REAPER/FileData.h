/*****************************************************************//**
 * @file   FileData.h
 * @brief  �t�@�C���f�[�^�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace inr {
	/** �t�@�C����� */
	class FileData {
	public:
		/**
		 * @brief								�R���X�g���N�^
		 * @param path					�p�X
		 * @param filename			�t�@�C����
		 * @param format				�t�H�[�}�b�g
		 */
		FileData(std::string path, std::string filename, std::string format);
		/**
		 * @brief								�f�X�g���N�^
		 */
		~FileData() = default;
		/**
		 * @brief								�p�X�̎擾
		 * @return							�p�X��Ԃ�
		 */
		inline std::string Path() { return _path; }
		/**
		 * @brief								�t�@�C�����̎擾
		 * @return							�t�@�C������Ԃ�
		 */
		inline std::string FileName() { return _fileName; }
		/**
		 * @brief								�t�H�[�}�b�g�̎擾
		 * @return							�t�H�[�}�b�g��Ԃ�
		 */
		inline std::string Format() { return _format; }
		/**
		 * @brief								�S���̎擾
		 * @return							�S����Ԃ�
		 */
		inline std::string AllPath() { return _path + _fileName + _format; }
	private:
		std::string _path;			//!< �p�X
		std::string _fileName;	//!< �t�@�C����
		std::string _format;		//!< �t�H�[�}�b�g
	};
}

