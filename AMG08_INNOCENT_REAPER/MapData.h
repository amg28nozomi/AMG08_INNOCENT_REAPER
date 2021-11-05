/*****************************************************************//**
 * @file   MapData.h
 * @brief  マップデータクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <string>
#include <vector>

using std::pair;
using std::tuple;

namespace inr {
  /** 二重インクルード防止 */
  class Game;
  /** マップデータ */
  class MapData {
  public:
    /**
     * @brief              コンストラクタ
     */
    MapData();
    /**
     * @brief              コンストラクタ
     * @param mapSize      マップサイズ(左辺:幅　右辺:高さ)
     * @param chipCount    チップ数(0:総数　1:横　2:縦)
     * @param chipSize     マップチップのサイズ(左辺:幅　右辺:高さ)
     * @param mapSizeLayer レイヤー数
     * @param fileName     ファイル名
     * @param mapData      マップチップの配置情報が格納されたコンテナ
     * @param chipType     マップチップの当たり判定が格納されたコンテナ
     */
    MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
    /**
     * @brief              デストラクタ
     */
    ~MapData() = default;
    /**
     * @brief              マップサイズ(幅)の取得
     * @return             マップサイズ(幅)を返す
     */
    inline int MapSizeWidth() { return _mapSizeW; }
    /**
     * @brief              マップサイズ(高さ)の取得
     * @return             マップサイズ(高さ)を返す
     */
    inline int MapSizeHeight() { return _mapSizeH; }
    /**
     * @brief              総チップ数の取得
     * @return             総チップ数を返す
     */
    inline int ChipCount() { return _chipCount; }
    /**
     * @brief              マップサイズの幅(チップ数)の取得
     * @return             マップサイズの幅(チップ数)を返す
     */
    inline int ChipCountWidth() { return _chipCountW; }
    /**
     * @brief              マップサイズの高さ(チップ数)の取得
     * @return             マップサイズの高さ(チップ数)を返す
     */
    inline int ChipCountHeight() { return _chipCountH; }
    /**
     * @brief              マップチップの幅の取得
     * @return             マップチップの幅を返す
     */
    inline int ChipSizeWidth() { return _chipSizeW; }
    /**
     * @brief              マップチップの高さの取得
     * @return             マップチップの高さを返す
     */
    inline int ChipSizeHeight() { return _chipSizeH; }
    /**
     * @brief              レイヤー数の取得
     * @return             レイヤー数を返す
     */
    inline int MapSizeLayer() { return _mapSizeLayer; }
    /**
     * @brief              ファイル名の取得
     * @return             ファイル名を返す
     */
    inline std::string FileName() { return _fileName; }
    /**
     * @brief             指定したマップチップの配置情報を取得
     * @param element     取得したい要素
     * @return            対応するマップチップの配置情報を返す
     */
    int MapDatas(int element) { return _mapDatas[element]; }
    /**
     * @brief             マップチップの配置情報の取得
     * @return            マップチップの配置情報を返す
     */
    std::vector<int> MapDatas() { return _mapDatas; }
    /**
     * @brief             当たり判定の取得
     * @return            当たり判定を格納するコンテナを返す
     */
    std::vector<int> ChipType() { return _chipType; }
    /**
     * @brief             マップサイズの登録
     * @param mapSizeW    幅
     * @param mapSizeH    高さ
     */
    void SetMapSize(int mapSizeW, int mapSizeH);
    /**
     * @brief             チップカウントの登録
     * @param count       チップ数
     * @param width       マップサイズの幅(チップ数)
     * @param height      マップサイズの高さ(チップ数)
     */
    void SetChipCount(int count, int width, int height);
    /**
     * @brief             マップチップサイズの登録
     * @param width       マップチップの幅
     * @param height      マップチップの高さ
     */
    void SetChipSize(int width, int height);
    /**
     * @brief             レイヤーの登録
     * @param layers      レイヤー数
     */
    inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
    /**
     * @brief             ファイル名の登録
     * @param fileName    ファイル名
     */
    inline void SetFillName(std::string fileName) { _fileName = fileName; }
    /**
     * @brief             配置情報の登録
     * @param mapDatas    配置情報が登録されたint型の動的配列
     */
    inline void SetMapDatas(std::vector<int> mapDatas) { _mapDatas = mapDatas; }
    /**
     * @brief             当たり判定の登録
     * @param chipType    当たり判定が登録されたint型の動的配列
     */
    inline void SetChipType(std::vector<int> chipType) { _chipType = chipType; }
  private:
    int _mapSizeW;              //!< マップサイズの幅
    int _mapSizeH;              //!< マップサイズ高さ
    int _chipCount;             //!< 総チップ数
    int _chipCountW;            //!< マップサイズの幅(チップ数)
    int _chipCountH;            //!< マップチップの高さ(チップ数)
    int _chipSizeW;             //!< マップチップの幅
    int _chipSizeH;             //!< マップチップの高さ
    int _mapSizeLayer;          //!< レイヤー
    std::string _fileName;      //!< 読み込みファイル名
    std::vector<int> _mapDatas; //!< マップチップの配置情報
    std::vector<int> _chipType; //!< 当たり判定があるマップチップ
  };
}