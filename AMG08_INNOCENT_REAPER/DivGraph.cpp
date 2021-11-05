/*****************************************************************//**
 * @file   DivGraph.cpp
 * @brief  DxLib::LoadDivGraphŠÖ”‚Å“Ç‚İ‚Ş‰æ‘œî•ñ
 *
 * @author —é–ØŠóŠC
 * @date   October 2021
 *********************************************************************/
#include "DivGraph.h"

namespace graph{

  DivGraph::DivGraph(std::string fileName, int xNum, int yNum, int allNum, int xSize, int ySize) :
    _filename(fileName), _xNum(xNum), _yNum(yNum), _allNum(allNum), _xSize(xSize), _ySize(ySize), _handles() {
  }

  DivGraph::DivGraph() {
    // Šeí‰Šú‰»
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