/*****************************************************************//**
 * @file   GimmickServer.cpp
 * @brief  ギミックサーバクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Game.h"

namespace inr {

	GimmickServer::GimmickServer(Game& game) : _game(game) {
		// 初期化
		Clear();
		_isDel = false;
	}

	GimmickServer::~GimmickServer() {
		Clear();	// 解放
	}

	void GimmickServer::Process() {
		if (_isDel == true) Del();			// フラグがオンの場合は消去処理呼び出し
		for (auto gimmick : _gimmicks) gimmick->Process();	// ギミックの更新処理呼び出し
	}

	void GimmickServer::Draw() {
		for (auto gimmick : _gimmicks) gimmick->Draw();			// ギミックの描画処理呼び出し
	}

	void GimmickServer::Add(std::shared_ptr<GimmickBase> gimmick) {
		_gimmicks.emplace_back(std::move(gimmick));					// コンテナに登録
	}

	void GimmickServer::Del() {
		// コンテナ
		std::vector<std::shared_ptr<GimmickBase>> gimmicks;
		// 消去判定が偽のギミックのみコンテナに登録
		for (auto g : _gimmicks) {
			if (g->IsDelete() == true) continue;	// 消去フラグがある場合は登録を行わない
			gimmicks.emplace_back(std::move(g));	// コンテナに登録する
		}
		_gimmicks.swap(gimmicks);	// 交換
		_isDel = false;						// 消去完了
	}
}
