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
	// リソースサーバ
	class ResourceServer {
	public:
		// リソース読み込み用情報を管理する連想配列
		using DivGraphMap = std::unordered_map<std::string, DivGraph>;
		// リソースの初期化
		static void Init();
		// リソースの解放
		static void Release();
		// コンテナの解放
		static void ClearGraphLists();
		// リソース情報の読み込み(引数:読み込み用情報)
		static void LoadGraphList(const DivGraphMap& divGraphMap);
		// リソース情報の読み込み(引数:読み込み用情報)
		// 画像ファイルのグラフィックハンドルを特定のコンテナにまとめる
		static void SetLoadGraph(const DivGraphMap& divGraphMap);
		// 指定したグラフィックハンドルの取得(引数1:キー　引数2:画像番号)
		static int GetHandles(const std::string& key, int no = 0);
		// 指定したグラフィックを管理するコンテナの取得(引数1:キー　引数2:ハンドルを受け取るためのコンテナ)
		static bool GetHandles(const std::string& key, std::vector<int>& handls);
		// 指定したグラフィックの総分割数を取得(引数;キー)
		static int GetAllNum(const std::string& key);
	private:
		// リソースを管理する連想配列
		static DivGraphMap _graphlists;
	};
}