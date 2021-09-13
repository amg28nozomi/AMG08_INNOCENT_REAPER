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
		_effects.clear();
		_addEffects.clear();
	}

	void EffectServer::Init() {
		_update = false;
		Clears();
	}

	void EffectServer::Process() {
		// �v�f����ł͂Ȃ��ꍇ�A�v�f��S�ĈڐA����
		if (_addEffects.empty() != true) {
			for(auto&& add : _addEffects) _effects.emplace_back(std::move(add)); 	// �R���e�i��S�ڐA����
			_addEffects.clear();
		}
		IsDelete();
		_update = true;
		for (auto&& eff : _effects) eff->Process();
		_update = false;
	}

	void EffectServer::Draw() {
		for (auto&& eff : _effects) eff->Draw();
	}

	void EffectServer::Add(std::unique_ptr<EffectBase> eff) {
		// �X�V���s���Ă���ꍇ�͈ꎞ�I�Ɋi�[
		switch (_update) {
		case true:
			_addEffects.emplace_back(std::move(eff));
			return;
		case false:
			_effects.emplace_back(std::move(eff));
			return;
		}
	}

	void EffectServer::IsDelete() {
		std::vector<std::unique_ptr<EffectBase>> alives;
		for (auto&& eff : _effects) {
			if (eff->IsDel() == true) continue;
			alives.emplace_back(std::move(eff));
		}
		_effects.clear();
		for (auto&& eff : alives) _effects.emplace_back(std::move(eff));
	}
}