#include "ResourceServer.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <DxLib.h>

namespace graph {

	ResourceServer::DivGraphMap ResourceServer::_graphlists;

	DivGraph::DivGraph(std::string fillname, int xnum, int ynum, int allnum, int xsize, int ysize) : 
		_fillname(fillname), _xNum(xnum), _yNum(ynum), _allNum(allnum), _xSize(xsize), _ySize(ysize), _handles(){
	}

	DivGraph::~DivGraph() {
	}

	std::string& DivGraph::GetFill() {
		std::string* fillname = &_fillname;

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

			LoadDivGraph(dg.GetFill().c_str(), allNum, dg.GetXnum(), dg.GetYnum(), dg.GetXsize(), dg.GetYsize(),dg.GetHandls().data());
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
}