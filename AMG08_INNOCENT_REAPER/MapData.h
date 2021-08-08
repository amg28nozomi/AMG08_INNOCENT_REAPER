#pragma once
#include <tuple>
#include <string>
#include <vector>

using std::pair;
using std::tuple;

namespace inr {

	class MapData {
	public:
		MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
		~MapData() = default;

		inline pair<int, int> GetMapSize() { return std::make_pair(_mapSizeW, _mapSizeH); }
		inline tuple<int, int, int> GetChipCount() { return std::make_tuple(_chipCount, _chipSizeW, _chipSizeH); }
		inline pair<int, int> GetChipSize() { return std::make_pair(_chipSizeW, _chipSizeH); }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFileName() { return _fileName; }
	private:
		int _mapSizeW;		// �}�b�v�T�C�Y�̕�
		int _mapSizeH;		// �}�b�v�T�C�Y����
		int _chipCount;		// ���`�b�v��
		int _chipCountW;	// 
		int _chipCountH;	//  
		int _chipSizeW;		// 
		int _chipSizeH;		// 
		int _mapSizeLayer;	// 
		std::string _fileName;	// �ǂݍ��݃t�@�C����
		std::vector<int> _maoDatas;	// �}�b�v�`�b�v�̔z�u���
		std::vector<int> _chipType;	// �����蔻�肪����}�b�v�`�b�v
	};
}

