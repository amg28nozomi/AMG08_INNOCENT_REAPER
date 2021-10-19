/*****************************************************************//**
 * @file   SoundData.h
 * @brief  �T�E���h���
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>

namespace se{
	/** �T�E���h�f�[�^ */
	class SoundData {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param	filename	�t�@�C����
		 * @param	type			�Đ��^�C�v
		 */
		SoundData(std::string filename, int type);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~SoundData() = default;
		/**
		 * @brief						�t�@�C�����̎擾
		 * @return					�t�@�C������Ԃ�
		 */
		std::string& GetFill();
		/**
		 * @brief						�T�E���h�n���h���̎Q�Ƃ��擾
		 * @return					�T�E���h�n���h���̎Q�Ƃ�Ԃ�
		 */
		inline int& GetHandleP() { return _handle; }
		/**
		 * @brief						�T�E���h�n���h���̎擾
		 * @return					�T�E���h�n���h����Ԃ�
		 */
		inline int GetHandle() { return _handle; }
		/**
		 * @brief						�Đ��^�C�v�̎擾
		 * @return					�Đ��^�C�v��Ԃ�
		 */
		inline int GetPlayType() { return _playType; }
	private:
		std::string _filename;	//!< �t�@�C����/�p�X��
		int _handle;						//!< �T�E���h�n���h��
		int _playType;					//!< �Đ��^�C�v
	};
}
