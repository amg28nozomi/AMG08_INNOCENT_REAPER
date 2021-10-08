/*****************************************************************//**
 * \file   GimmickServer.cpp
 * \brief  ギミックサーバクラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
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
	// 更新
	void GimmickServer::Process() {
		if (_isDel == true) Del();	// フラグがオンの場合は消去処理呼び出し
		for (auto gimmick : _gimmicks) gimmick->Process();	// ギミックの更新処理呼び出し
	}
	// 描画
	void GimmickServer::Draw() {
		for (auto gimmick : _gimmicks) gimmick->Draw();		// ギミックの描画処理呼び出し
	}
	// ギミックの登録
	void GimmickServer::Add(std::shared_ptr<GimmickBase> gimmick) {
		// コンテナに登録
		_gimmicks.emplace_back(std::move(gimmick));
	}
	// ギミックの消去
	void GimmickServer::Del() {
		// コンテナ
		std::vector<std::shared_ptr<GimmickBase>> gimmicks;
		// 消去判定が偽のギミックのみコンテナに登録
		for (auto g : _gimmicks) {
			if (g->IsDelete() == true) continue;
			gimmicks.emplace_back(std::move(g));
		}
		_gimmicks.swap(gimmicks);	// 交換
		_isDel = false;				// 消去完了
	}
}
