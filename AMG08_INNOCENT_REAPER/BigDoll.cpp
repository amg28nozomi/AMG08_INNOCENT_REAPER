#include "BigDoll.h"
#include "SoundServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoulSkin.h"
#include <memory>

namespace {
	constexpr auto BIG_EMPTY_WIDTH = 200;
	constexpr auto BIG_EMPTY_HEIGHT = 100;

	constexpr auto PATROL_MAX = 280;
	constexpr auto PATROL_VECTOR = 40.0 / 60.0;

	constexpr auto BIG_DOLL_VITAL = 40;

	constexpr auto STAY_MAX = 60;

	// 攻撃
	constexpr auto ATTACK_VECTOR_MIN = 1.0;
	constexpr auto ATTACK_VECTOR_MAX = 2.5;
	constexpr auto ADD_VECTOR = 0.1;
}

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::BIG_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::BIG_EMPTY, key::SOUND_NUM);
		_position = { 0, 0 };
		_atkVec = 0;
	}

	BigDoll::~BigDoll() {

	}

	void BigDoll::Init() {
		// 当たり判定
		_mainCollision = { _position, enemy::BIG_WIDTH / 2, enemy::BIG_HEIGHT / 2, true };
		_collisions = {
			// 抜け殻
			{enemy::BIG_EMPTY, {_position, BIG_EMPTY_WIDTH / 2, BIG_EMPTY_WIDTH / 2, 0, BIG_EMPTY_HEIGHT / 2, true}},
		};
		_motionKey = {
			{ enemy::BIG_EMPTY, {40, 0}},
			
			{ enemy::red::BIG_WAKEUP, {20, 0}},
			{ enemy::red::BIG_IDOL, {15, 0}},
			{ enemy::red::BIG_PATROL, {20, 0}},
			{ enemy::red::BIG_TACKLE, {20, 0}},
			{ enemy::red::BIG_HIPDROP, {28, 0}},

			{ enemy::blue::BIG_WAKEUP, {20, 0}},
			{ enemy::blue::BIG_IDOL, {15, 0}},
			{ enemy::blue::BIG_PATROL, {20, 0}},
			{ enemy::blue::BIG_ESCAPE, {20, 0}},
		};
	}

	void BigDoll::Process() {
		ObjectBase::Process();	// 重力処理
		_moveVector.GetPX() = 0;	// 初期化

		AnimationCount();
		Action();
		StateUpdate();

	}

	void BigDoll::Draw() {

	}


	void BigDoll::HipDrop() {

	}

	void BigDoll::StateUpdate() {
		if (_soul == nullptr) return;
		if (0 < _stay) {
			--_stay;
			return;
		}

		switch (_aState) {
		case ActionState::IDOL:
			PatrolOn();
			return;
		case ActionState::PATROL:
			// 左移動
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				return;
				// 右移動
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				return;
			}
			// 移動量が0の場合は待機状態に遷移する
			_aState = ActionState::IDOL;
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = enemy::red::BIG_IDOL;
				break;
			case soul::BLUE:
				_divKey.first = enemy::blue::BIG_IDOL;
				break;
			}
			_stay = STAY_MAX;
			return;
		case ActionState::ATTACK:
			// 左移動
			if (_actionX < 0) {
				if(-ATTACK_VECTOR_MAX < _atkVec ) _atkVec += ADD_VECTOR;
				_actionX += _atkVec;
				_moveVector.GetPX() = -_atkVec;
				if (0 < _actionX) _actionX = 0;

			} else if (0 < _actionX) {
				if (_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX -= _atkVec;
				_moveVector.GetPX() = _atkVec;
				if (_actionX < 0) _actionX = 0;
			}
			if(_actionX == 0) ChangeIdol();
			return;
		case ActionState::ESCAPE:
			// 左移動
			if (_actionX < 0) {
				if (-ATTACK_VECTOR_MAX < _atkVec) _atkVec += ADD_VECTOR;
				_actionX += _atkVec;
				_moveVector.GetPX() = -_atkVec;
				if (0 < _actionX) _actionX = 0;

			}
			else if (0 < _actionX) {
				if (_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX -= _atkVec;
				_moveVector.GetPX() = _atkVec;
				if (_actionX < 0) _actionX = 0;
			}
			if (_actionX == 0) ChangeIdol();
			return;
			// 起き上がりモーション時
		case ActionState::WAKEUP:
			// 起き上がりアニメーションの再生が終わったなら、巡回状態に遷移する
			if (IsAnimationMax()) {
				ChangeIdol();
				_stay = STAY_MAX;
#ifdef _DEBUG
				_searchBox.GetbDrawFlg() = true;
#endif

			}
			return;
		}
	}

	void BigDoll::Move() {
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}

	void BigDoll::Death() {
		ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
		EnemyBase::Death();
	}

	void BigDoll::SetParameter(ObjectValue objValue) {
		EnemyBase::SetParameter(objValue);
		if (_oValue.SoulType() == 0) {	// 魂が空の場合は抜け殻になる
			ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
			_aCount = AnimationCountMax();	// カウンタをマックスにする
			return;	// 処理を抜ける
		}
		auto soul_n = std::make_shared<SoulSkin>(_game.GetGame());
		// auto sn = std::static_pointer_cast<SoulSkin>(soul_n);
		switch (_oValue.SoulType()) {
		case 1:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::red::BIG_PATROL);
			break;
		case 2:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::blue::BIG_PATROL);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error：EnemyBase->SetParameter　ObjectValueの_soulTypeの値が不正です\n");
#endif
			break;
		}
		_soul = soul_n;
		_game.GetObjectServer()->Add(std::move(soul_n));
	}

	void BigDoll::PatrolOn() {
		if (_soul == nullptr) return;
		EnemyBase::PatrolOn();

		// 魂の色に応じてキーを切り替え
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::BIG_PATROL : _divKey.first = enemy::red::BIG_PATROL;

		if (_actionX == 0) {
			if (_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}

	void BigDoll::ChangeIdol() {
		_searchBox.GetCollisionFlgB() = false;	// 当たり判定を切る
		std::string nextkey = "";
		switch (_soul->SoulColor()) {
		case soul::RED:
			nextkey = enemy::red::BIG_IDOL;
			break;
		case soul::BLUE:
			nextkey = enemy::blue::BIG_IDOL;
			break;
		}
		ChangeState(ActionState::IDOL, nextkey);
	}
}