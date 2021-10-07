#include "ResourceServer.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <DxLib.h>

namespace graph {

	ResourceServer::DivGraphMap ResourceServer::_graphlists;

	DivGraph::DivGraph(std::string filename, int xnum, int ynum, int allnum, int xsize, int ysize) : 
		_filename(filename), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles(){
	}

	DivGraph::DivGraph() {
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
		std::string* fillname = &_filename;

		return *fillname;
	}


	void ResourceServer::Init() {
		ClearGraphLists();
	}

	void ResourceServer::Release() {
		ClearGraphLists();
	}

	void ResourceServer::ClearGraphLists() {
		for (auto&& grl: _graphlists) {
			for (auto gh : grl.second.GetHandls()) {
				DeleteGraph(gh);
			}
		}
		_graphlists.clear();
	}

	void ResourceServer::LoadGraphList(const DivGraphMap& divGraphMap) {
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;	// 参照元のキー情報
			auto it = _graphlists.find(key);  
			if (it != _graphlists.end()) {
				continue;
			}

			auto dg = dgm.second;
			auto allNum = dg.GetAllNum();
			
			dg.GetHandls().resize(allNum);

			LoadDivGraph(dg.GetFile().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
			_graphlists.emplace(key.c_str(), dg);
		}

	}

	int ResourceServer::GetHandles(const std::string& key, int no) {
		auto it = _graphlists.find(key);
		// イテレータが見つからなかった場合は終了
		if (it == _graphlists.end()) {
			return -1;
		}
		auto graph = it->second.GetHandle().at(no);
		return graph;
	}

	bool ResourceServer::GetHandles(const std::string& key, std::vector<int>& handls) {
		auto it = _graphlists.find(key);
		if (it == _graphlists.end()) {
			return false;
		}
		handls.resize(it->second.GetHandls().size());
		handls = it->second.GetHandle();
		return true;
	}

	/*int ResourceServer::GetSize(const std::string& key) {
		auto it = _graphlists.find(key);
		if()
	}*/

	int ResourceServer::GetAllNum(const std::string& key) {
		auto it = _graphlists.find(key);
		if (it == _graphlists.end()) {
			return -1;
		}
		return it->second.GetAllNum();
	}

	// 一枚画像の複数登録処理
	void ResourceServer::SetLoadGraph(const DivGraphMap& divGraphMap) {
		for (auto&& dgm : divGraphMap) {
			auto& key = dgm.first;	// キーの取得
			auto it = _graphlists.find(key);	// 検索
			if (it != _graphlists.end()) continue;	// 既に登録済みの場合はスキップ

			DivGraph dgraph = dgm.second;
			auto&& dghandle = dgraph.GetHandls();
			dghandle.resize(dgraph.GetAllNum());
			for (auto i = 0; i < dgraph.GetAllNum(); ++i) {
				std::string number;
				if (i < 10) {
					number = "0" + std::to_string(i);
				} else {
					number = std::to_string(i);
				}
				std::string fn = dgraph.GetFile() + number + ".png";
				LoadDivGraph(fn.c_str(), 1, 1, 1, dgraph.GetXsize(), dgraph.GetYsize(), &dghandle.at(i));
			}
			_graphlists.emplace(dgm.first, dgraph);
		}
	}

	//void ResourceServer::SetLoadGraph(std::string gkey, std::string path, std::string filename, int maxsize, int xsize, int ysize) {
	//	auto it = _graphlists.find(gkey);
	//	if (it != _graphlists.end()) return;
	//	// 空のデータを作成
	//	DivGraph dgraph = { path + filename, 1, 1, maxsize, xsize, ysize};
	//	auto&& dghandle = dgraph.GetHandls();
	//	dghandle.resize(maxsize);
	//	for (auto i = 0; i < maxsize; ++i) {
	//		std::string number;
	//		if (i < 10) {
	//			number = "0" + std::to_string(i);
	//		}
	//		else {
	//			number = std::to_string(i);
	//		}
	//		std::string fn = dgraph.GetFile() + number  + ".png";
	//		LoadDivGraph(fn.c_str(), 1, 1, 1, xsize, ysize, &dghandle.at(i));
	//	}
	//	_graphlists.emplace(gkey, dgraph);
	//}
}