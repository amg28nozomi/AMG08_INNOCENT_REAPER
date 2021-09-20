#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Game.h"

namespace inr {

	GimmickServer::GimmickServer(Game& game) : _game(game) {
		Clear();
	}

	GimmickServer::~GimmickServer() {
		Clear();
	}

	void GimmickServer::Process() {
		for (auto gimmick : _gimmicks) gimmick->Process();
	}

	void GimmickServer::Draw() {
		for (auto gimmick : _gimmicks) gimmick->Draw();
	}


	void GimmickServer::Add(std::shared_ptr<GimmickBase> gimmick) {
		_gimmicks.emplace_back(std::move(gimmick));
	}
}
