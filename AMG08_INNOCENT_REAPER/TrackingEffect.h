#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {

	class ObjectBase;
	// 追跡処理があり
	class TrackingEffect : public EffectBase {
	public:
		TrackingEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// 生成座標および、生存フレーム数、分割数、一画像当たりの待ち時間
		~TrackingEffect() = default;

		void Process() override;
		// 追従処理の設定（引数1:追従対象　引数2:修正値X　引数:修正値Y）
		void Set(ObjectBase* target, double fixx = 0, double fixy = 0);
	private:
		ObjectBase* _target;	// 追跡目標
		Vector2 _fix;
		// double _moveMax;	// 移動量
	};
}

