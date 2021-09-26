#include "EffectServer.h"
#include "EffectBase.h"

namespace inr {

	EffectServer::EffectServer(Game& game) : _game(game) {
		Clears();
	}

	EffectServer::~EffectServer() {
		Clears();
	}

	void EffectServer::Clears() {
		_effectsF.clear();
		_effectsB.clear();
		_addFormer.clear();
		_addBack.clear();
	}

	void EffectServer::Init() {
		_update = false;
		_isAdd = false;
		_del = false;
		Clears();
	}

	void EffectServer::Process() {
		// 要素が空ではない場合、要素を全て移植する
		if (_isAdd == true) IsEmpty();
		IsDelete();
		_update = true;
		for (auto&& eff : _effectsB) eff->Process();
		for (auto&& eff : _effectsF) eff->Process();
		_update = false;
	}

	void EffectServer::DrawBack() {
		for (auto&& eff : _effectsB) eff->Draw();
	}

	void EffectServer::DrawFormer() {
		for (auto&& eff : _effectsF) eff->Draw();
	}

	void EffectServer::Add(std::unique_ptr<EffectBase> eff, int type) {
		// 更新を行っている場合は一時的に格納
		switch (_update) {
		case true:
			_isAdd = true;
			switch (type) {
			case effect::type::FORMER:
				_addFormer.emplace_back(std::move(eff));
				return;
			case effect::type::BACK:
				_addBack.emplace_back(std::move(eff));
				return;
			default:
				return;
			}
			return;
		case false:
			switch (type) {
			case effect::type::FORMER:
				_effectsF.emplace_back(std::move(eff));
				return;
			case effect::type::BACK:
				_effectsB.emplace_back(std::move(eff));
				return;
			default:
				return;
			}
		}
	}

	bool EffectServer::IsEmpty() {	
		if (_addFormer.empty() != true) {
			for(auto&& eff : _addFormer) _effectsF.emplace_back(std::move(eff));
			_addFormer.clear();
		}

		if (_addBack.empty() != true) {
			for(auto&& eff : _addBack)  _effectsB.emplace_back(std::move(eff));
			_addBack.clear();
		}
		_isAdd = false;
		return true;
	}

	void EffectServer::IsDelete() {
		if (_del != true) return;	// フラグがオンの時以外は実行しない
		std::vector<std::unique_ptr<EffectBase>> alives;
		for (auto&& eff : _effectsF) {
			if (eff->IsDel() == true) continue;
			alives.emplace_back(std::move(eff));
		}
		_effectsF.clear();
		for (auto&& eff : alives) _effectsF.emplace_back(std::move(eff));
		alives.clear();
		for (auto&& eff : _effectsB) {
			if (eff->IsDel() == true) continue;
			alives.emplace_back(std::move(eff));
		}
		_effectsB.clear();
		for (auto&& eff : alives) _effectsB.emplace_back(std::move(eff));
		_del = false;
	}
}