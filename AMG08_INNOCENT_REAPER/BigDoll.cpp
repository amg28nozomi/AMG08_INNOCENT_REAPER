#include "BigDoll.h"
#include "SoundServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "SoulSkin.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "Block.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"
#include "EffectBase.h"
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

	constexpr auto TACKLE_MAX = 1200;

	constexpr auto GIVE_STAY = 120;
}

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::BIG_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::BIG_EMPTY, key::SOUND_NUM);
		_position = { 500, 500 };
		_atkVec = 0;

		Init();
	}

	BigDoll::~BigDoll() {

	}

	void BigDoll::Init() {
		// 当たり判定
		_mainCollision = { _position, enemy::BIG_WIDTH / 2, enemy::BIG_HEIGHT / 2, true };
		_searchBox = { _position, 260, 260, enemy::BIG_HEIGHT / 2 + 30, enemy::BIG_HEIGHT / 2, true};
		_collisions = {
			// 抜け殻
			{enemy::BIG_EMPTY, {_position, BIG_EMPTY_WIDTH / 2, BIG_EMPTY_WIDTH / 2, 0, BIG_EMPTY_HEIGHT, true}},
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
			{ enemy::blue::BIG_ESCAPE, {20, 50}},
		};
		_moveCount = 0;
	}

	void BigDoll::Process() {
		ObjectBase::Process();	// 重力処理
		_moveVector = { 0, 0 };	// 初期化

		AnimationCount();
		Action();
		StateUpdate();
		Move();
		Attack();
		PositionUpdate();
	}

	void BigDoll::Attack() {
		if (_soul == nullptr || _aState == ActionState::WAKEUP) return;	// 魂がない場合は処理をスキップ
		auto&& player = _game.GetObjectServer()->GetPlayer();
		auto playerBox = player->GetMainCollision();	// プレイヤーの当たり判定を取得

		// ギミック（レバー）と衝突したか？
		if (_aState == ActionState::ATTACK || _aState == ActionState::ESCAPE) {
			auto gs = _game.GetGimmickServer()->GetGimmicks();
			for (auto gg : gs) {
				if (gg->GimmickType() == gimmick::DOOR) continue;	// レバーではない場合はスキップ
				if (_mainCollision.HitCheck(gg->GetMainCollision())) {
					switch (gg->GimmickType()) {
					case gimmick::BLOCK:
						if (std::dynamic_pointer_cast<Block>(gg)->IsBreak() != gimmick::block::BRAKE_ON) {
							_stay = 30;
							std::string key = "";
							if (_soul->SoulColor() == soul::BLUE) key = enemy::blue::BIG_IDOL;
							else key = enemy::red::BIG_IDOL;
							ChangeState(ActionState::IDOL, key);
							std::dynamic_pointer_cast<Block>(gg)->Break(); 
						}
						continue;
					case gimmick::LEVER:
						if (_aState != ActionState::ATTACK) continue;
						std::dynamic_pointer_cast<Lever>(gg)->OpenDoor();
						continue;
					default:
						continue;
					}
				}
			}
		}

		// 自身の当たり判定と接触判定を行う
		auto dmb = DamageBox(10);
		if (dmb.HitCheck(playerBox)) {
			player->Damage(SearchPosition());
			return;
		}
		// 攻撃状態の場合は追加で処理を行う
		//if (_aState == ActionState::ATTACK) {
		//	auto box = _collisions.find(_divKey.first);
		//	// 接触している場合は自機の衝突処理を呼び出す
		//	if (box->second.HitCheck(playerBox)) {
		//		// 進行方向とは同方向に対象を飛ばす
		//		player->Damage(SearchPosition());
		//	}
		//}
	}

	void BigDoll::HipDrop() {

	}

	void BigDoll::StateUpdate() {
		if (_soul == nullptr) return;
		if (0 < _stay) {
			--_stay;
			if (_stay == 0) {
				_isAction = false;
				_mainCollision.GetCollisionFlgB() = true;
			}
			return;
		}

		if (0 < _moveCount) {
			--_moveCount;
			if (_aState != ActionState::ATTACK && _aState != ActionState::ESCAPE) _moveCount = 0;
		}

		switch (_aState) {
		case ActionState::IDOL:
			if (_mainCollision.GetCollisionFlg() != true) _mainCollision.GetCollisionFlgB() = true;
			PatrolOn();
			return;
		case ActionState::PATROL:
			// 右移動
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				if (0 < _patrolX) _patrolX = 0;
				return;
				// ←移動
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				if (_patrolX < 0) _patrolX = 0;
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
			_searchBox.GetCollisionFlgB() = false;	// 索敵を切る
			return;
		case ActionState::ATTACK:
			// 立っていてかつ、攻撃SEを鳴らしていない時限定で実装する
			if (_stand == true && _moveCount == 0) { 
				PlaySe(enemy::bigdoll::SE_TACKLE);
				_moveCount = 30;
			}

			// 右移動
			if (_actionX < 0) {
				if(_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX += _atkVec;
				_moveVector.GetPX() = -_atkVec;
				if (0 < _actionX) _actionX = 0;

			} else if (0 < _actionX) {
				if (-ATTACK_VECTOR_MAX < _atkVec) _atkVec += ADD_VECTOR;
				_actionX -= _atkVec;
				_moveVector.GetPX() = _atkVec;
				if (_actionX < 0) _actionX = 0;
			}
			if(_actionX == 0) ChangeIdol();
			return;
		case ActionState::ESCAPE:
			if (_stand == true && _moveCount == 0) {
				PlaySe(enemy::bigdoll::SE_TACKLE);
				_moveCount = 30;
			}
			// 左移動
			if (_actionX < 0) {
				_actionX += (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = -(enemy::ESCAPE_VECTOR / 30);
				if (0 < _actionX) _actionX = 0;

			}
			else if (0 < _actionX) {
				_actionX -= (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = (enemy::ESCAPE_VECTOR / 30);
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
				// _mainCollision.GetCollisionFlgB() = true;
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
			auto embox = _collisions.find(enemy::BIG_EMPTY);
			embox->second.Update(_position, _direction);
			ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
			_aCount = AnimationCountMax();	// カウンタをマックスにする
			_changeGraph = true;
			_mainCollision.GetCollisionFlgB() = false;
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

	void BigDoll::PositionUpdate() {
		// 移動ベクトルYに加速度を代入
		_moveVector.GetPY() = _gravity;
		// マップチップにめり込んでいる場合は座標を修正
		_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction);
		GimmickCheck(_moveVector);
		_position = _position + _moveVector;	// 位置座標を更新

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		auto col = _collisions.find(enemy::BIG_EMPTY);
		col->second.Update(_position, _direction);

		if (_soul == nullptr && IsAnimationMax() == true) {
			_mainCollision.Swap(col->second);
#ifdef _DEBUG
			_searchBox.GetbDrawFlg() = false;
#endif

		}
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

	void BigDoll::AttackOn() {
		if (_aState != ActionState::ATTACK) {
			ChangeState(ActionState::ATTACK, enemy::red::BIG_TACKLE);
			_searchBox.GetCollisionFlgB() = true;

			PlaySe(enemy::bigdoll::SE_TACKLE_VOICE);

			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX = -TACKLE_MAX;
				return;
			case enemy::MOVE_RIGHT:
				_actionX = TACKLE_MAX;
				return;
			}
		}
	}

	AABB BigDoll::NowCollision(std::string key) {
		if (_soul != nullptr) return _mainCollision;
		auto it = _collisions.find(enemy::BIG_EMPTY);
		// 現在のアクション状態はボックスを修正する必要があるか？
		return it->second;
	}

	void BigDoll::EscapeOn() {
		if (_aState != ActionState::ESCAPE) {
			ChangeState(ActionState::ESCAPE, enemy::blue::BIG_ESCAPE);
			_searchBox.GetCollisionFlgB() = true;

			PlaySe(enemy::bigdoll::SE_ESCAP_VOICE);

			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX = enemy::ESCAPE_MAX;
				return;
			case enemy::MOVE_RIGHT:
				_actionX = -enemy::ESCAPE_MAX;
				return;
			}
		}
	}

	void BigDoll::ChangeIdol() {
		_atkVec = 0;
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

	void BigDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// 現在の急所がある座標を算出
		auto vitalPart = VitalPart(_mainCollision, BIG_DOLL_VITAL);
		auto player = _game.GetObjectServer()->GetPlayer();
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
					_searchBox.GetCollisionFlgB() = false;	// 一時的に索敵判定を切る

					PlaySe(enemy::bigdoll::SE_DOWN);

					auto hiteff = std::make_unique<EffectBase>(_game.GetGame(), effect::S_HIT, _position, 30);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hiteff), effect::type::FORMER);

					_soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
					_moveCount = 0;
					_stay = 0;
					_isAction = false;

					_mainCollision.GetCollisionFlgB() = false;

					// 自機が保有する魂が所持上限に到達している場合は所有権を手放す
					if (player->IsSoulMax()) {
						_soul->OwnerNull();
						_soul.reset();	// 所有権を手放す
						return;
					}
					player->SoulCatch(std::move(_soul));	// 魂の所有権をプレイヤーに譲渡
					return;
				}
			}
		}
		// 魂を与えられるか？
		if (ckey == PKEY_GIVE) {
			// プレイヤーは魂を所持しているか？
			if (player->HaveSoul()) {
				// 魂が空の場合にボックスが接触したら
				if (_soul == nullptr) {
					// 接触時の判定はAABBで行う（奪うアクションとは違い、向きによる制限なし）
					if (NowCollision(_divKey.first).HitCheck(acollision)) {
						// プレイヤーを取得
						auto player = _game.GetObjectServer()->GetPlayer();
						_soul = player->GiveSoul();	// プレイヤ―から対象の魂を受け取る
						_soul->Inactive();	// 魂を非活性状態にする
						PlaySe(key::SOUND_PLAYER_GIVE_TRUE);
						_searchBox.GetCollisionFlgB() = false;

						switch (_soul->SoulColor()) {
						case soul::RED:
							ChangeState(ActionState::WAKEUP, enemy::red::BIG_WAKEUP);
							return;
						case soul::BLUE:
							ChangeState(ActionState::WAKEUP, enemy::blue::BIG_WAKEUP);
							return;
						}
						return;
					}
				}
			}
		}
	}
}