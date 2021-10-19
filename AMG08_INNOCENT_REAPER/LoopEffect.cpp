/*****************************************************************//**
 * @file   LoopEffect.cpp
 * @brief  ループエフェクト（エフェクトベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "LoopEffect.h"
#include "ObjectBase.h"
#include "Game.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"

namespace inr {

	LoopEffect::LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction) : EffectBase(game, gh, spawnpos, maxFrame, direction) {
		_owner = nullptr;	// 所有者の初期化
	}

	void LoopEffect::Process() {
		// 終了判定が真の場合は消去処理呼び出し
		if (IsEnd() == true) _game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
		Move();						// 追従処理
		// アニメーション管理
		if (_count == (_alive - 1)) {
			_count = 0;			// 初期化
		} else ++_count;	// 加算
	}

	bool LoopEffect::IsEnd() {
		if (_delete == true && _owner == nullptr) return false;		// 既に消去フラグはtrueになっている
		// 所有者が死亡している場合
		if (_owner->IsDead() == true) {
			_delete = true;		// フラグオン
			return true;			// 消去する
		}
		return false;				// 消去しない
	}

	void LoopEffect::SetOwner(ObjectBase* owner) {
		_owner = std::move(owner);	// 登録
	}

	bool LoopEffect::Move() {
		if (_owner == nullptr) return false;	// 所有者なし
		auto movepos = _owner->GetPosition();	// 所有者の座標を取得
		_position = movepos;									// 座標を代入
		return true;
	}
}
