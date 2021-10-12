/*****************************************************************//**
 * \file   ResourceServer.h
 * \brief  �f�B�u�O���t�N���X
 *		   DxLib::LoadDivGraph�֐��œǂݍ��މ摜�̏��
 *		   ���\�[�X�T�[�o�N���X
 *		   DxLib::LoadDivGraph�Ŏ擾�����O���t�B�b�N�n���h���̊Ǘ����s��
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
	//// DxLib::LoadDivGraph�֐��p�̃N���X
	class DivGraph {
	public:
		// �R���X�g���N�^(����1:�t�@�C�����@����2:�������ɑ΂��镪�����@����3:�c�����ɑ΂��镪�����@����4:�摜�̑��������@����5:�������ꂽ�摜�̉����@����6:�������ꂽ�摜�̏c��)
		DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize);
		// �R���X�g���N�^
		DivGraph();
		// �f�X�g���N�^
		~DivGraph();
		// �t�@�C�����̎擾
		std::string& GetFile();
		// �������������̎擾
		inline int GetXnum() { return _xNum; }
		// �c�����������̎擾
		inline int GetYnum() { return _yNum; }
		// �摜�̉����̎擾
		inline int GetXsize() { return _xSize; }
		// �摜�̏c���̎擾
		inline int GetYsize() { return _ySize; }
		// ���������̎擾
		inline int GetAllNum() { return _allNum; }
		// �O���t�B�b�N�n���h���R���e�i�̎Q��
		inline std::vector<int>& GetHandls() { return _handles; }
		// �O���t�B�b�N�n���h���R���e�i�̎擾
		inline std::vector<int> GetHandle() { return _handles; }
		// �O���t�B�b�N�n���h���R���e�i�̃��T�C�Y
		inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }
		// �O���t�B�b�N�n���h���R���e�i�̌���
		inline void SwapHandles(std::vector<int> newhandles) { _handles.swap(newhandles); }
	private:
		std::string  _filename;		// �t�@�C����
		int _xNum;					// ������(��)
		int _yNum;					// ������(��)
		int _allNum;				// ��������
		int _xSize;					// �������̃T�C�Y(x)
		int _ySize;					// �������̃T�C�Y(y)
		std::vector<int> _handles;	// �O���t�B�b�N�n���h���i�[�p�̃R���e�i
	};
	// ���\�[�X�T�[�o
	class ResourceServer {
	public:
		// ���\�[�X�ǂݍ��ݗp�����Ǘ�����A�z�z��
		using DivGraphMap = std::unordered_map<std::string, DivGraph>;
		// ���\�[�X�̏�����
		static void Init();
		// ���\�[�X�̉��
		static void Release();
		// �R���e�i�̉��
		static void ClearGraphLists();
		// ���\�[�X���̓ǂݍ���(����:�ǂݍ��ݗp���)
		static void LoadGraphList(const DivGraphMap& divGraphMap);
		// ���\�[�X���̓ǂݍ���(����:�ǂݍ��ݗp���)
		// �摜�t�@�C���̃O���t�B�b�N�n���h�������̃R���e�i�ɂ܂Ƃ߂�
		static void SetLoadGraph(const DivGraphMap& divGraphMap);
		// �w�肵���O���t�B�b�N�n���h���̎擾(����1:�L�[�@����2:�摜�ԍ�)
		static int GetHandles(const std::string& key, int no = 0);
		// �w�肵���O���t�B�b�N���Ǘ�����R���e�i�̎擾(����1:�L�[�@����2:�n���h�����󂯎�邽�߂̃R���e�i)
		static bool GetHandles(const std::string& key, std::vector<int>& handls);
		// �w�肵���O���t�B�b�N�̑����������擾(����;�L�[)
		static int GetAllNum(const std::string& key);
	private:
		// ���\�[�X���Ǘ�����A�z�z��
		static DivGraphMap _graphlists;
	};
}