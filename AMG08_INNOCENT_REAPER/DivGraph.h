/*****************************************************************//**
 * @file   DivGraph.h
 * @brief  DxLib::LoadDivGraph関数で読み込む画像情報
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <string>
#include <vector>

namespace graph {
  /** DxLib::LoadDivGraph関数用のデータ */
  class DivGraph {
  public:
    /**
     * @brief          コンストラクタ
     * @param fileName ファイル名
     * @param xNum     横向きに対する分割数
     * @param yNum     縦向きに対する分割数
     * @param allNum   画像の総分割数
     * @param xSize    分割された画像の横幅
     * @param ySize    分割された画像の縦幅
     */
    DivGraph(std::string fileName, int xNum, int yNum, int allNum, int xSize, int ySize);
    /**
     * @brief          コンストラクタ
     */
    DivGraph();
    /**
     * @brief          デストラクタ
     */
    ~DivGraph();
    /**
     * @brief          ファイル名の取得
     * @return         ファイル名を返す
     */
    std::string& GetFile();
    /**
     * @brief          横向き分割数の取得
     * @return         横向き分割数の取得
     */
    inline int GetXnum() { return _xNum; }
    /**
     * @brief          縦向き分割数の取得
     * @return         縦向き分割数を返す
     */
    inline int GetYnum() { return _yNum; }
    /**
     * @brief          画像の横幅の取得
     * @return         横幅を返す
     */
    inline int GetXsize() { return _xSize; }
    /**
     * @brief          画像の縦幅の取得
     * @return         縦幅を返す
     */
    inline int GetYsize() { return _ySize; }
    /*
     * @brief          総分割数の取得
     * @return         総分割数を返す
     */
    inline int GetAllNum() { return _allNum; }
    /**
     * @brief          グラフィックハンドルコンテナの参照
     * @return         コンテナの参照を返す
     */
    inline std::vector<int>& GetHandls() { return _handles; }
    /**
     * @brief          グラフィックハンドルコンテナの取得
     * @return         コンテナを返す
     */
    inline std::vector<int> GetHandle() { return _handles; }
    /**
     * @brief          ハンドル用コンテナのリサイズ
     * @param maxNum   サイズ
     */
    inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }
    /**
     * @brief          ハンドル用コンテナのスワップ
     * @param handles  スワップ対象
     */
    inline void SwapHandles(std::vector<int> handles) { _handles.swap(handles); }
  private:
    std::string  _filename;    //!< ファイル名
    int _xNum;                 //!< 分割数(横)
    int _yNum;                 //!< 分割数(縦)
    int _allNum;               //!< 総分割数
    int _xSize;                //!< 分割時のサイズ(x)
    int _ySize;                //!< 分割時のサイズ(y)
    std::vector<int> _handles; //!< グラフィックハンドル格納用コンテナ
  };
}