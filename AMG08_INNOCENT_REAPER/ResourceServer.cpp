/*****************************************************************//**
 * @file   ResourceServer.cpp
 * @brief  DxLib::LoadDivGraphで取得したグラフィックハンドルの管理を行うリソースサーバ
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ResourceServer.h"
#include "DivGraph.h"
#include <unordered_map>
#include <memory>
#include <DxLib.h>

namespace graph {
  // 静的メンバの定義
  ResourceServer::DivGraphMap ResourceServer::_graphLists;

  void ResourceServer::Init() {
    ClearGraphLists();
  }

  void ResourceServer::Release() {
    ClearGraphLists();
  }

  void ResourceServer::ClearGraphLists() {
    // 登録されているグラフィックの解放
    for (auto&& grl: _graphLists) {
      // グラフィックハンドルの取り出し
      for (auto gh : grl.second.GetHandls()) {
        DeleteGraph(gh); // メモリ上から削除する
      }
    }
    _graphLists.clear(); // コンテナ初期化
  }

  void ResourceServer::LoadGraphList(const DivGraphMap& divGraphMap) {
    // キーとバリューを取り出し処理を行う
    for (auto&& dgm : divGraphMap) {
      auto& key = dgm.first;
      auto it = _graphLists.find(key); // 既に登録されているかの確認
      if (it != _graphLists.end()) {
        continue; // 登録されている
      }
      auto dg = dgm.second;          // 読み込み用情報の取り出し
      auto allNum = dg.GetAllNum();  // 画像の総分割数    
      dg.GetHandls().resize(allNum); // グラフィック格納用のコンテナをリサイズ
      // 画像の読み込みを行う
      LoadDivGraph(dg.GetFile().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
      _graphLists.emplace(key.c_str(), dg); // コンテナに登録する
    }
  }

  int ResourceServer::GetHandles(const std::string& key, int no) {
    auto it = _graphLists.find(key); // リソースの検索
    // 該当リソースは見つかったか？
    if (it == _graphLists.end()) {
      return -1; // ヒットしなかった
    }
    auto graph = it->second.GetHandle().at(no); // グラフィックハンドルの取得
    return graph; // ハンドルを返す
  }

  bool ResourceServer::GetHandles(const std::string& key, std::vector<int>& handles) {
    auto it = _graphLists.find(key); // リソースの検索
    // 該当リソースは見つかったか？
    if (it == _graphLists.end()) {
      return false;                  // ヒットしなかった
    }
    // ハンドル情報のコピー
    handles.resize(it->second.GetHandls().size());
    handles = it->second.GetHandle();
    return true; // ヒットした
  }

  int ResourceServer::GetAllNum(const std::string& key) {
    auto it = _graphLists.find(key); // リソースの検索
    // 該当リソースは見つかったか？
    if (it == _graphLists.end()) {
      return -1; // ヒットしなかった
    }
    return it->second.GetAllNum(); // 総分割数を返す
  }

  void ResourceServer::SetLoadGraph(const DivGraphMap& divGraphMap) {
    // キーとバリューを取り出し処理を行う
    for (auto&& dgm : divGraphMap) {
      auto& key = dgm.first;           // キー
      auto it = _graphLists.find(key); // 検索
      if (it != _graphLists.end()) continue; // 既に登録済みの場合はスキップ

      DivGraph dGraph = dgm.second;             // 読み込み情報のコピー
      auto&& dGraphHandle = dGraph.GetHandls(); // コンテナの参照
      dGraphHandle.resize(dGraph.GetAllNum());  // サイス変更
      // 総分割数分画像を読み込む
      for (auto i = 0; i < dGraph.GetAllNum(); ++i) {
        std::string number; // 画像ファイル用
        // iが条件を見たいしているか
        if (i < 10) {
          // 満たしていない場合は先頭に"0"を足してstring型に変換
          number = "0" + std::to_string(i);
        } else {
          // 満たしている場合はそのままstring型に変換する
          number = std::to_string(i);
        }
        // ファイル名の設定
        std::string fn = dGraph.GetFile() + number + ".png";
        // 画像を読み込む
        LoadDivGraph(fn.c_str(), 1, 1, 1, dGraph.GetXsize(), dGraph.GetYsize(), &dGraphHandle.at(i));
      }
      _graphLists.emplace(dgm.first, dGraph); // コンテナに登録する
    }
  }
}