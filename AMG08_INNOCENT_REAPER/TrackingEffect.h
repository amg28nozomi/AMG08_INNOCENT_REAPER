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
		// �Ǐ]�����̐ݒ�i����1:�Ǐ]�Ώہ@����2:�C���lX�@����:�C���lY�j
		void Set(ObjectBase* target, double fixx = 0, double fixy = 0);
	private:
		ObjectBase* _target;	// �ǐՖڕW
		Vector2 _fix;
		// double _moveMax;	// �ړ���
	};
}

