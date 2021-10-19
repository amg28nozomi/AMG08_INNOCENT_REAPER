/*****************************************************************//**
 * @file   DivGraph.cpp
 * @brief  DxLib::LoadDivGraph関数で読み込む画像情報
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "DivGraph.h"

namespace graph{
	// コンストラクタ
	DivGraph::DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize) :
		_filename(filename), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles() {
	}
	// コンストラクタ
	DivGraph::DivGraph() {
		// 各種初期化
		_filename = "";
		_xNum = 0;
		_yNum = 0;
		_allNum = 0;
		_xSize = 0;
		_ySize = 0;
		_handles.clear();
	}
	// デストラクタ
	DivGraph::~DivGraph() {
	}
	// ファイル名の取得
	std::string& DivGraph::GetFile() {
		std::string* fillname = &_filename;
		return *fillname;
	}
}
