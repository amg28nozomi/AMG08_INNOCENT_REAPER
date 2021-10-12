/*****************************************************************//**
 * \file   LoopEffect.h
 * \brief  ループエフェクト（エフェクトベースのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {
	// 二重インクルード防止
	class ObjectBase;
	// 無限ループ処理有り
	class LoopEffect : public EffectBase {
	public:
		// コンストラクタ(引数1:ゲームクラスの参照　引数2:画像キー　引数3:生成地点　引数4:生存フレーム数　引数5:反転フラグ)
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		// デストラクタ
		~LoopEffect() = default;
		// 更新
		void Process() override;
		// 所有者の設定(引数:ObjectBase型のポインタ)
		void SetOwner(ObjectBase* owner);
	private:
		ObjectBase* _owner;		// 所有者
		// オブジェクトを削除するかの判定
		bool IsEnd();
		// 追従処理
		bool Move();
	};
}

