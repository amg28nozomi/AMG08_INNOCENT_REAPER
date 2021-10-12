/*****************************************************************//**
 * \file   ResourceServer.h
 * \brief  ディブグラフクラス
 *		   DxLib::LoadDivGraph関数で読み込む画像の情報
 *		   リソースサーバクラス
 *		   DxLib::LoadDivGraphで取得したグラフィックハンドルの管理を行う
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
	//// DxLib::LoadDivGraph関数用のクラス
	class DivGraph {
	public:
		// コンストラクタ(引数1:ファイル名　引数2:横向きに対する分割数　引数3:縦向きに対する分割数　引数4:画像の総分割数　引数5:分割された画像の横幅　引数6:分割された画像の縦幅)
		DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize);
		// コンストラクタ
		DivGraph();
		// デストラクタ
		~DivGraph();
		// ファイル名の取得
		std::string& GetFile();
		// 横向き分割数の取得
		inline int GetXnum() { return _xNum; }
		// 縦向き分割数の取得
		inline int GetYnum() { return _yNum; }
		// 画像の横幅の取得
		inline int GetXsize() { return _xSize; }
		// 画像の縦幅の取得
		inline int GetYsize() { return _ySize; }
		// 総分割数の取得
		inline int GetAllNum() { return _allNum; }
		// グラフィックハンドルコンテナの参照
		inline std::vector<int>& GetHandls() { return _handles; }
		// グラフィックハンドルコンテナの取得
		inline std::vector<int> GetHandle() { return _handles; }
		// グラフィックハンドルコンテナのリサイズ
		inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }
		// グラフィックハンドルコンテナの交換
		inline void SwapHandles(std::vector<int> newhandles) { _handles.swap(newhandles); }
	private:
		std::string  _filename;		// ファイル名
		int _xNum;					// 分割数(横)
		int _yNum;					// 分割数(横)
		int _allNum;				// 総分割数
		int _xSize;					// 分割時のサイズ(x)
		int _ySize;					// 分割時のサイズ(y)
		std::vector<int> _handles;	// グラフィックハンドル格納用のコンテナ
	};
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