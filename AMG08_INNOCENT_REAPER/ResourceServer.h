#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
	//// DxLib::LoadDivGraph�֐��p�̃N���X
	class DivGraph {
	public:
		DivGraph(std::string fillname, int xnum, int allnum, int ynum, int xsize, int ysize);
		~DivGraph();
		// �ȉ��Q�b�^�[

		//inline std::string GetFill() { return _fillname; }
		std::string& GetFill();

		inline int GetXnum() { return _xNum; }
		inline int GetYnum() { return _yNum; }
		inline int GetXsize() { return _xSize; }
		inline int GetYsize() { return _ySize; }
		inline int GetAllNum() { return _allNum; }
		inline std::vector<int>& GetHandls() { return _handles; }
		inline std::vector<int> GetHandle() { return _handles; }
		inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }

		inline void SwapHandles(std::vector<int> newhandles) { _handles.swap(newhandles); }

	private:
		std::string  _fillname;		// �t�@�C����
		int _xNum;		// ������(��)
		int _yNum;		// ������(��)
		int _allNum;	// ��������
		int _xSize;		// �������̃T�C�Y(x)
		int _ySize;		// �������̃T�C�Y(y)
		std::vector<int> _handles;		// �O���t�B�b�N�n���h���i�[�p
	};

	class ResourceServer {
	public:
		// �^���̕ʖ���`
		// using �ʖ� = �^���ŕʖ����`�ł���
		using DivGraphMap = std::unordered_map<std::string, DivGraph>;
		static void Init();		// ������
		static void Release();		// ���
		static void ClearGraphLists();
		static void LoadGraphList(const DivGraphMap& divGraphMap);	// �摜�̈ꊇ�ǂݍ���

		static int GetHandles(const std::string& key, int no = 0);
		static bool GetHandles(const std::string& key, std::vector<int>& handls);

		// static int GetSize(const std::string& key);		// �L�[�ɊY������z��̃T�C�Y���擾
		static int GetAllNum(const std::string& key);

	private:
		static DivGraphMap _graphlists;
	};
}