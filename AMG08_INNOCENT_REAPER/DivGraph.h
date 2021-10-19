/*****************************************************************//**
 * @file   DivGraph.h
 * @brief  DxLib::LoadDivGraph�֐��œǂݍ��މ摜���
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>
#include <vector>

namespace graph {
	/** DxLib::LoadDivGraph�֐��p�̃f�[�^ */
	class DivGraph {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 * @param					�t�@�C����
		 * @param					�������ɑ΂��镪����
		 * @param					�c�����ɑ΂��镪����
		 * @param					�摜�̑�������
		 * @param					�������ꂽ�摜�̉���
		 * @param					�������ꂽ�摜�̏c��
		 */
		DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize);
		/**
		 * @brief					�R���X�g���N�^
		 */
		DivGraph();
		/**
		 * @brief					�f�X�g���N�^
		 */
		~DivGraph();
		/**
		 * @brief					�t�@�C�����̎擾
		 * @return				�t�@�C������Ԃ�
		 */
		std::string& GetFile();
		/**
		 * @brief					�������������̎擾
		 * @return				�������������̎擾
		 */
		inline int GetXnum() { return _xNum; }
		/**
		 * @brief					�c�����������̎擾
		 * @return				�c������������Ԃ�
		 */
		inline int GetYnum() { return _yNum; }
		/**
		 * @brief					�摜�̉����̎擾
		 * @return				������Ԃ�
		 */
		inline int GetXsize() { return _xSize; }
		/**
		 * @brief					�摜�̏c���̎擾
		 * @return				�c����Ԃ�
		 */
		inline int GetYsize() { return _ySize; }
		/**
		 * @brief					���������̎擾
		 * @return				����������Ԃ�
		 */
		inline int GetAllNum() { return _allNum; }
		/**
		 * @brief					�O���t�B�b�N�n���h���R���e�i�̎Q��
		 * @return				�R���e�i�̎Q�Ƃ�Ԃ�
		 */
		inline std::vector<int>& GetHandls() { return _handles; }
		/**
		 * @brief					�O���t�B�b�N�n���h���R���e�i�̎擾
		 * @return				�R���e�i��Ԃ�
		 */
		inline std::vector<int> GetHandle() { return _handles; }
		/**
		 * @brief					�n���h���p�R���e�i�̃��T�C�Y
		 * @param maxNum	�T�C�Y
		 */
		inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }
		/**
		 * @brief					�n���h���p�R���e�i�̃X���b�v
		 * @param					�X���b�v�Ώ�
		 */
		inline void SwapHandles(std::vector<int> newhandles) { _handles.swap(newhandles); }
	private:
		std::string  _filename;			//!< �t�@�C����
		int _xNum;									//!< ������(��)
		int _yNum;									//!< ������(�c)
		int _allNum;								//!< ��������
		int _xSize;									//!< �������̃T�C�Y(x)
		int _ySize;									//!< �������̃T�C�Y(y)
		std::vector<int> _handles;	//!< �O���t�B�b�N�n���h���i�[�p�R���e�i
	};
}

