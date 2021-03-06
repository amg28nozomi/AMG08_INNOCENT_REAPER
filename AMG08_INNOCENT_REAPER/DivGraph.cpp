/*****************************************************************//**
 * @file   DivGraph.cpp
 * @brief  DxLib::LoadDivGraph関数で読み込む画像情報
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "DivGraph.h"

namespace graph{

  DivGraph::DivGraph(std::string fileName, int xNum, int yNum, int allNum, int xSize, int ySize) :
    _filename(fileName), _xNum(xNum), _yNum(yNum), _allNum(allNum), _xSize(xSize), _ySize(ySize), _handles() {
  }

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

  DivGraph::~DivGraph() {
  }

  std::string& DivGraph::GetFile() {
    std::string* fillName = &_filename;
    return *fillName;
  }
}