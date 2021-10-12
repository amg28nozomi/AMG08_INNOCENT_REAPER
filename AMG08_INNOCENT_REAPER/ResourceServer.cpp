/*****************************************************************//**
 * \file   ResourceServer.cpp
 * \brief  ディブグラフクラス
 *		   DxLib::LoadDivGraph関数で読み込む画像の情報
 *		   リソースサーバクラス
 *		   DxLib::LoadDivGraphで取得したグラフィックハンドルの管理を行う
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#include "ResourceServer.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <DxLib.h>

namespace graph {
	// 静的メンバの定義
	ResourceServer::DivGraphMap ResourceServer::_graphlists;
	// コンストラクタ
	DivGraph::DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize) : 
		_filename(filename), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles(){
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
	// リソースの初期化
	void ResourceServer::Init() {
		ClearGraphLists();	// コンテナの解放
	}
	// リソースの解放
	void ResourceServer::Release() {
		ClearGraphLists();	// コンテナの解放
	}
	// コンテナの解放
	void ResourceServer::ClearGraphLists() {
		// 登録されているグラフィックの解放
		for (auto&& grl: _graphlists) {
			// グラフィックハンドルの取り出し
			for (auto gh : grl.second.GetHandls()) {
				DeleteGraph(gh);	// メモリ上から削除する
			}
		}
		_graphlists.clear();	// コンテナ初期化
	}
	// リソースの読み込み
	void ResourceServer::LoadGraphList(const DivGraphMap& divGraphMap) {
		// キーとバリューを取り出し処理を行う
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;				// キー情報
			auto it = _graphlists.find(key);	// 既に登録されているかの確認
			if (it != _graphlists.end()) {
				continue;	// 登録されている
			}
			auto dg = dgm.second;			// 読み込み用情報の取り出し
			auto allNum = dg.GetAllNum();	// 画像の総分割数		
			dg.GetHandls().resize(allNum);	// グラフィック格納用のコンテナをリサイズ
			// 画像の読み込みを行う
			LoadDivGraph(dg.GetFile().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
			_graphlists.emplace(key.c_str(), dg);	// コンテナに登録する
		}

	}
	// グラフィックハンドルの取得
	int ResourceServer::GetHandles(const std::string& key, int no) {
		auto it = _graphlists.find(key);	// リソースの検索
		// 該当リソースは見つかったか？
		if (it == _graphlists.end()) {
			return -1;	// ヒットしなかった
		}
		auto graph = it->second.GetHandle().at(no);		// グラフィックハンドルの取得
		return graph;	// ハンドルを返す
	}
	// 該当するグラフィックハンドルを管理するコンテナの取得
	bool ResourceServer::GetHandles(const std::string& key, std::vector<int>& handls) {
		auto it = _graphlists.find(key);	// リソースの検索
		// 該当リソースは見つかったか？
		if (it == _graphlists.end()) {
			return false;	// ヒットしなかった
		}
		// ハンドル情報のコピー
		handls.resize(it->second.GetHandls().size());
		handls = it->second.GetHandle();
		return true;		// ヒットした
	}
	// 指定したグラフィックの総分割数を取得
	int ResourceServer::GetAllNum(const std::string& key) {
		auto it = _graphlists.find(key);	// リソースの検索
		// 該当リソースは見つかったか？
		if (it == _graphlists.end()) {
			return -1;	// ヒットしなかった
		}
		return it->second.GetAllNum();	// 総分割数を返す
	}
	// リソース情報の読み込み
	void ResourceServer::SetLoadGraph(const DivGraphMap& divGraphMap) {
		// キーとバリューを取り出し処理を行う
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;					// キー
			auto it = _graphlists.find(key);		// 検索
			if (it != _graphlists.end()) continue;	// 既に登録済みの場合はスキップ

			DivGraph dgraph = dgm.second;			// 読み込み情報のコピー
			auto&& dghandle = dgraph.GetHandls();	// コンテナの参照
			dghandle.resize(dgraph.GetAllNum());	// サイス変更
			// 総分割数分画像を読み込む
			for (auto i = 0; i < dgraph.GetAllNum(); ++i) {
				std::string number;		// 画像ファイル用
				// iが条件を見たいしているか
				if (i < 10) {
					// 満たしていない場合は先頭に"0"を足してstring型に変換
					number = "0" + std::to_string(i);
				} else {
					// 満たしている場合はそのままstring型に変換する
					number = std::to_string(i);
				}
				// ファイル名の設定
				std::string fn = dgraph.GetFile() + number + ".png";
				// 画像を読み込む
				LoadDivGraph(fn.c_str(), 1, 1, 1, dgraph.GetXsize(), dgraph.GetYsize(), &dghandle.at(i));
			}
			_graphlists.emplace(dgm.first, dgraph);		// コンテナに登録する
		}
	}
}