#include "LoopEffect.h"
#include "ObjectBase.h"
#include "Game.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"

namespace inr {

	LoopEffect::LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction) : EffectBase(game, gh, spawnpos, maxFrame, direction) {
		_owner = nullptr;
	}

	void LoopEffect::Process() {
		IsEnd();
		Move();

		if (_count == (_alive - 1)) {
			_count = 0;
		} else ++_count;
	}

	bool LoopEffect::IsEnd() {
		if (_delete == true && _owner == nullptr) return false;
		if (_owner->IsDead() == true) {
			_delete = true;
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
		}
		return true;
	}

	void LoopEffect::SetOwner(ObjectBase* owner) {
		_owner = std::move(owner);
	}

	bool LoopEffect::Move() {
		if (_owner == nullptr) return false;
		auto movepos = _owner->GetPosition();
		_position = movepos;
		return true;
	}
}
