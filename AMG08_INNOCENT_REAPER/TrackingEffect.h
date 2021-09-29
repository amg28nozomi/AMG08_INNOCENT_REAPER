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
		// void Draw() override;
		void Set(ObjectBase* target, double fixx = 0, double fixy = 0);	// 修正値
	private:
		ObjectBase* _target;	// 追跡目標
		Vector2 _fix;
	};
}

