/*****************************************************************//**
 * @file   DivGraph.cpp
 * @brief  DxLib::LoadDivGraph�֐��œǂݍ��މ摜���
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "DivGraph.h"

namespace graph{

	DivGraph::DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize) :
		_filename(filename), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles() {
	}

	DivGraph::DivGraph() {
		// �e�평����
		_filename = "";
		_xNum = 0;
		_yNum = 0;
		_allNum = 0;
		_xSize = 0;
		_ySize = 0;
		_handles.clear();
	}

	DivGraph::~DivGraph() {
	}

	std::string& DivGraph::GetFile() {
		std::string* fillname = &_filename;
		return *fillname;
	}
}
