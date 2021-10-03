#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {

	class ObjectBase;

	// �������[�v�����L��
	class LoopEffect : public EffectBase {
	public:
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		~LoopEffect() = default;

		void Process() override;
		void SetOwner(std::shared_ptr<ObjectBase> owner);	// ���L�҂̐ݒ�

	private:
		std::shared_ptr<ObjectBase> _owner;	// ���L��
		bool IsEnd();
		bool Move();
	};
}

