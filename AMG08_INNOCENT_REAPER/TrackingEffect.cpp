#include "TrackingEffect.h"
#include "ObjectBase.h"

namespace inr {

	TrackingEffect::TrackingEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction) : EffectBase(game, gh, spawnpos, maxFrame, direction) {
		_target = nullptr;	// Š—LŽÒ‚Í‚¢‚È‚¢‚æ
		_fix = { 0, 0 };
	}

	void TrackingEffect::Set(ObjectBase* target, double fixx, double fixy) {
		_target = target;
		_fix = { fixx, fixy };
	}

	void TrackingEffect::Process() {
		auto pos = _target->GetPosition();
		switch (_direction) {
		case false:
			pos.GetPX() += _fix.GetX();
			pos.GetPY() = pos.GetY() + _fix.GetY();
			break;
		case true:
			pos.GetPX() -= _fix.GetX();
			pos.GetPY() = pos.GetY() + _fix.GetY();
			break;
		}
		_position = pos;
		EffectBase::Process();
	}
}