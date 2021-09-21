#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {

	class ObjectBase;
	// �ǐՏ���������
	class TrackingEffect : public EffectBase {
	public:
		TrackingEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// �������W����сA�����t���[�����A�������A��摜������̑҂�����
		~TrackingEffect() = default;

		void Process() override;
		// void Draw() override;
		void Set(ObjectBase* target, int fix = 0);	// �C���l
	private:
		ObjectBase* _target;	// �ǐՖڕW
		int _fix;
	};
}
