#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace graph {
	//// DxLib::LoadDivGraph関数用のクラス
	class DivGraph {
	public:
		DivGraph(std::string fillname, int xnum, int allnum, int ynum, int xsize, int ysize);
		~DivGraph();
		// 以下ゲッター

		//inline std::string GetFill() { return _fillname; }
		std::string& GetFill();

		inline int GetXnum() { return _xNum; }
		inline int GetYnum() { return _yNum; }
		inline int GetXsize() { return _xSize; }
		inline int GetYsize() { return _ySize; }
		inline int GetAllNum() { return _allNum; }
		inline std::vector<int>& GetHandls() { return _handles; }
		inline std::vector<int> GetHandle() { return _handles; }
		inline void ResizeHandles(int maxNum) { _handles.resize(maxNum); }

		inline void SwapHandles(std::vector<int> newhandles) { _handles.swap(newhandles); }

	private:
		std::string  _fillname;		// ファイル名
		int _xNum;		// 分割数(横)
		int _yNum;		// 分割数(横)
		int _allNum;	// 総分割数
		int _xSize;		// 分割時のサイズ(x)
		int _ySize;		// 分割時のサイズ(y)
		std::vector<int> _handles;		// グラフィックハンドル格納用
	};

	class ResourceServer {
	public:
		// 型名の別名定義
		// using 別名 = 型名で別名を定義できる
		using DivGraphMap = std::unordered_map<std::string, DivGraph>;
		static void Init();		// 初期化
		static void Release();		// 解放
		static void ClearGraphLists();
		static void LoadGraphList(const DivGraphMap& divGraphMap);	// 画像の一括読み込み

		static int GetHandles(const std::string& key, int no = 0);
		static bool GetHandles(const std::string& key, std::vector<int>& handls);

		// static int GetSize(const std::string& key);		// キーに該当する配列のサイズを取得
		static int GetAllNum(const std::string& key);

	private:
		static DivGraphMap _graphlists;
	};
}