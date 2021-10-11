/*****************************************************************//**
 * \file   FileData.h
 * \brief  �t�@�C���f�[�^�N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace inr {
	// �t�@�C�����
	class FileData {
	public:
		// �R���X�g���N�^(����1:�p�X�@����2:�t�@�C�����@����3:�t�H�[�}�b�g)
		FileData(std::string path, std::string filename, std::string format);
		// �f�X�g���N�^
		~FileData() = default;
		// �p�X�̎擾
		inline std::string Path() { return _path; }
		// �t�@�C�����̎擾
		inline std::string FileName() { return _fileName; }
		// �t�H�[�}�b�g�̎擾
		inline std::string Format() { return _format; }
		// �S���擾
		inline std::string AllPath() { return _path + _fileName + _format; }
	private:
		std::string _path;			// �p�X
		std::string _fileName;		// �t�@�C����
		std::string _format;		// �t�H�[�}�b�g
	};
}

