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
		void SetOwner(ObjectBase* owner);	// ���L�҂̐ݒ�

	private:
		ObjectBase* _owner;	// ���L��
		bool IsEnd();
		bool Move();
	};
}

