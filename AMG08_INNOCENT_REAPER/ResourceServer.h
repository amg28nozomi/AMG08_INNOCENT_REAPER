/*****************************************************************//**
 * @file   ResourceServer.h
 * @brief  DxLib::LoadDivGraphで取得したグラフィックハンドルの管理を行うリソースサーバ
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
  /** 二重インクルード防止 */
  class DivGraph;
  /** リソースサーバ */
  class ResourceServer {
  public:
    /** 読み込み用情報を管理する連想配列 */
    using DivGraphMap = std::unordered_map<std::string, DivGraph>;
    /**
     * @brief             リソースの初期化
     */
    static void Init();
    /**
     * @brief             リソースの解放
     */
    static void Release();
    /**
     * @brief             コンテナ
     */
    static void ClearGraphLists();
    /**
     * @brief             リソース情報の読み込み
     * @param divGraphMap 読み込み用情報
     */
    static void LoadGraphList(const DivGraphMap& divGraphMap);
    /**
     * @brief             リソース情報の読み込み(調整あり)
     * @param divGraphMap 読み込み用情報
     */
    static void SetLoadGraph(const DivGraphMap& divGraphMap);
    /**
     * @brief             指定したグラフィックハンドルの取得
     * @param key         キー
     * @param no          ハンドル番号
     * @return            グラフィックハンドルを返す
     *                    ヒットしなかった場合は-1を返す
     */
    static int GetHandles(const std::string& key, int no = 0);
    /**
     * @brief             指定したグラフィックを管理するコンテナの取得
     * @param key         キー
     * @param handles     ハンドル格納用コンテナ
     * @return            取得に成功した場合はtrueを返す
     *                    失敗した場合はfalseを返す
     */
    static bool GetHandles(const std::string& key, std::vector<int>& handles);
    /**
     * @brief             指定したグラフィックの総分割数を取得
     * @param key         キー
     * @return            取得に成功した場合はtrueを返す
     *                    取得に失敗した場合はfalseを返す
     */
    static int GetAllNum(const std::string& key);
  private:
    static DivGraphMap _graphLists; //!< リソースを管理する連想配列
  };
}