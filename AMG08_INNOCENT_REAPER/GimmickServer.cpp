#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Game.h"

namespace inr {

	GimmickServer::GimmickServer(Game& game) : _game(game) {
		Clear();
		_isDel = false;
	}

	GimmickServer::~GimmickServer() {
		Clear();
	}

	void GimmickServer::Process() {
		if (_isDel == true) Del();	// �t���O���I���̏ꍇ�͏��������Ăяo��
		for (auto gimmick : _gimmicks) gimmick->Process();
	}

	void GimmickServer::Draw() {
		for (auto gimmick : _gimmicks) gimmick->Draw();
	}


	void GimmickServer::Add(std::shared_ptr<GimmickBase> gimmick) {
		_gimmicks.emplace_back(std::move(gimmick));
	}

	void GimmickServer::Del() {
		_isDel = false;	// �t���O��؂�
		std::vector<std::shared_ptr<GimmickBase>> gimmicks;
		for (auto g : _gimmicks) {
			if (g->IsDelete() == true) continue;
			gimmicks.emplace_back(std::move(g));
		}
		_gimmicks.swap(gimmicks);
	}
}
