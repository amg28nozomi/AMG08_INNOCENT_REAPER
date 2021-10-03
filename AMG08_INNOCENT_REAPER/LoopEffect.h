#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {

	class ObjectBase;

	// 無限ループ処理有り
	class LoopEffect : public EffectBase {
	public:
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		~LoopEffect() = default;

		void Process() override;
		void SetOwner(ObjectBase* owner);	// 所有者の設定

	private:
		ObjectBase* _owner;	// 所有者
		bool IsEnd();
		bool Move();
	};
}

