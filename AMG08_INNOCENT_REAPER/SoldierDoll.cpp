#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "Player.h"
#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "EffectBase.h"
#include "EffectServer.h"
#include "ModeServer.h"
#include "ModeMain.h"

// α仮
#include "SoulSkin.h"
#include <memory>

namespace {
	constexpr auto START_POS_X = 500;
	constexpr auto START_POS_Y = 400;

	// 幅・高さ
	constexpr auto SOLDIER_W = 60;
	constexpr auto SOLDIER_H = 120;

	// 通常状態の当たり判定(修正用)
	constexpr auto SF_HEIGHT = 20;
	constexpr auto SOLDIER_BOX_W = SOLDIER_W / 2;
	constexpr auto SOLDIER_BOX_H1 = SOLDIER_H / 2 - SF_HEIGHT;
	constexpr auto SOLDIER_BOX_H2 = SOLDIER_H / 2 + SF_HEIGHT;

	// 干渉範囲 
	constexpr auto SOLDIER_VITAL = 20;
	// 当たり判定
	constexpr auto COLLISION_W = 60;

	constexpr auto EMPTY_W = 250;
	constexpr auto EMPTY_H = 100;

	// 索敵範囲
	constexpr auto SEARCH_X = 200 + (SOLDIER_BOX_W * 2);	//　260
	constexpr auto SEARCH_Y1 = SOLDIER_BOX_H1;
	constexpr auto SEARCH_Y2 = SOLDIER_BOX_H2;

	// 巡回範囲(最大Vector)
	constexpr auto PATROL_VECTOR = 1.0;
	constexpr auto PATROL_MAX = 280;

	// 攻撃用範囲
	constexpr auto ATTACK_W1 = SOLDIER_BOX_W + 30;
	constexpr auto ATTACK_W2 = SOLDIER_W / 2;
	constexpr auto ATTACK_H = 30;

	constexpr auto FRAME = 30;

	// アイドルモーション時間
	constexpr auto STAY_MAX = 60;	//　stay
	constexpr auto GIVE_STAY = 33 * 2;

	constexpr auto IDOL_FRAME = 33;
}

namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::SOLDIER_EMPTY, key::SOUND_NUM);
		_position = { 0, 0 };

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_patrolX = 0;
		_aInterval = 0;
		_direction = false;
		_changeGraph = true;
		_drawStop = false;

		Init();
	}

	SoldierDoll::~SoldierDoll() {
	}

	void SoldierDoll::Init() {
		// メインの当たり判定を初期化
		// _mainCollision = { _position, SOLDIER_BOX_W, SOLDIER_BOX_W, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_mainCollision = { _position, 30, 60, true };

		// _searchBox = { _position, SEARCH_X, SEARCH_X, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_searchBox = { _position, SEARCH_X, SEARCH_X, 70, 60, true };

		/*_searchBox = { { _mainCollision.GetMin().GetX() - SEARCH_X, _mainCollision.GetMin().GetY() - SEARCH_Y }, {
						_mainCollision.GetMax().GetX() + SEARCH_X, _mainCollision.GetMax().GetY() + SEARCH_Y } };*/
		// 各種判定の格納（攻撃判定・）
		_collisions = {
			{ enemy::SOLDIER_EMPTY, { _position, 60, 60, -25, 60 , true}},	// 抜け殻時の情報
			// { enemy::SOLDIER_EMPTY, { _position, 65, 60, -45, 80 , true}},	// 抜け殻時の情報
			{ enemy::red::SOLDIER_ATTACK, {_position, ATTACK_W1, ATTACK_W2, ATTACK_H, ATTACK_H, true}},
		};

		_motionKey =
		{	{ enemy::SOLDIER_EMPTY, {19 * 3, 0}},

			{ enemy::red::SOLDIER_WAKEUP, {17 * 3, 0}},
			{ enemy::red::SOLDIER_IDOL, {11 * 3, 0}},
			{ enemy::red::SOLDIER_PATROL, {13 * 3, 13 * 3}},
			{ enemy::red::SOLDIER_ATTACK, {11 * 3, 50}},	// SE有り

			{ enemy::blue::SOLDIER_WAKEUP, {17 * 3, 0}},
			{ enemy::blue::SOLDIER_IDOL, {11 * 3, 0}},
			{ enemy::blue::SOLDIER_PATROL, {13 * 3, 13 * 3}},
			{ enemy::blue::SOLDIER_ESCAPE, {13 * 3, 0}}
		};

		PatrolOn();
	}

	void SoldierDoll::Process() {
		ObjectBase::Process();
		_moveVector.GetPX() = 0;

		// アニメーションが終わっていない場合はカウントを増やす
		AnimationCount();

		Patrol();
		Action();
		Attack();

		// 当たり判定を取得

		Move();
		PositionUpdate();
	}

	void SoldierDoll::StateUpdate() {
		if (_soul == nullptr) return;	// 魂がない場合は処理を終了
		switch (_soul->SoulColor()) {
		case soul::RED:
			if (_aInterval) {
				--_aInterval;
			}
			return;
		case soul::BLUE:
			if (_aInterval) {
				--_aInterval;
			}
			return;
		}
	}

	void SoldierDoll::Action() {
		if (_aState == ActionState::WAKEUP || _isAction == true) return;
		// プレイヤーを発見できるか
		if (SearchPlayer() == true) {
			_isAction = true;
			ChangeIdol(IDOL_FRAME);

			auto eye_light = std::make_unique<EffectBase>(_game.GetGame(), effect::soldier::OMEN, _position, effect::soldier::OMEN_NUMS * 3, _direction);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eye_light), effect::type::FORMER);
			switch (_soul->SoulColor()) {
			case soul::RED:
				PlaySe(enemy::soldier::ATTACK_VOICE);
				break;
			case soul::BLUE:
				PlaySe(enemy::soldier::ESCAPE_VOICE);
				break;
			}
		}
		// 発見できなかった場合は移動処理を行う
		if (_soul == nullptr) _actionX = 0;
	}

	void SoldierDoll::Patrol() {
		if (_soul == nullptr) return;	// 魂が空の場合は処理を行わない
		if (_stay != 0) {
			--_stay;	// 待機モーション中はカウンタを減らして処理を抜ける
			if (_stay == 0) { 
				_mainCollision.SetCollisionFlag() = true;
			}
			return;
		}

		switch (_aState) {
		case ActionState::IDOL:
			if (_mainCollision.GetCollisionFlag() != true) _mainCollision.SetCollisionFlag() = true;	
			if (_isAction == true) {
				switch (_soul->SoulColor()) {
					case soul::RED:
						AttackOn();
						_isAction = false;
						return;
					case soul::BLUE:
						EscapeOn();
						_isAction = false;
						return;
				}
			}
			PatrolOn();
			_searchBox.SetCollisionFlag() = true;
			return;
		case ActionState::PATROL:
			// 左移動
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				return;
			// 右移動
			} else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				return;
			}
			// 移動量が0の場合は待機状態に遷移する
			_aState =  ActionState::IDOL;
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = enemy::red::SOLDIER_IDOL;
				break;
			case soul::BLUE:
				_divKey.first = enemy::blue::SOLDIER_IDOL;
				break;
			}
			_stay = STAY_MAX;
			return;
		// 起き上がりモーション時
		case ActionState::WAKEUP:
			// 起き上がりアニメーションの再生が終わったなら、巡回状態に遷移する
			if (AnimationCountMax() == true) {
				ChangeIdol(GIVE_STAY);
#ifdef _DEBUG
				_searchBox.SetDrawFlag() = true;
#endif
			}
			return;
		}


		// 魂が空の場合はゼロにする
		//if (_aState == ActionState::EMPTY) _patrolX = 0;
		//else if (_aState != ActionState::PATROL && _aState != ActionState::ESCAPE && _aState!= ActionState::IDOL) {
		//	PatrolOn();
		//}
		//if (_aState == ActionState::PATROL) {
		//	// 左移動
		//	if (_patrolX < 0) {
		//		_moveVector.GetPX() = PATROL_VECTOR;
		//		_patrolX += PATROL_VECTOR;
		//		return;
		//	}
		//	else if (0 < _patrolX) {
		//		_moveVector.GetPX() = -PATROL_VECTOR;
		//		_patrolX -= PATROL_VECTOR;
		//	}
		//	// 移動量が0になった（端に付いた）場合は待機状態に遷移
		//	if (static_cast<int>(_patrolX) == 0) {
		//		ActionState::IDOL;
		//		switch (_sState) {
		//		case SoulState::BLUE:
		//			_divKey.first = enemy::blue::SOLDIER_IDOL;
		//			break;
		//		case SoulState::RED:
		//			_divKey.first = enemy::red::SOLDIER_IDOL;
		//			break;
		//		}
		//		_stay = STAY_MAX;

		//		if (_direction == enemy::MOVE_LEFT) _patrolX = -PATROL_MAX;
		//		else _patrolX = PATROL_MAX;
		//	}
		//}
	}

	void SoldierDoll::Move() {
		// 移動ベクトルに応じて、向きを変更
		// 移動量に応じて向きを変更
		

		if (_actionX < 0) { 
			_direction = enemy::MOVE_LEFT; }
		else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;
		/*if (_aState == ActionState::PATROL && _aCount % GetSoundFrame(_divKey.first) == 0 && IsAnimationMax() != true) {
			PlaySe(enemy::soldier::MOVE);
		}*/

		

		Escape();

		/*if (_moveVector.GetX() < 0) {
			_direction = true;
		}
		else if (0 < _moveVector.GetX()) {
			_direction = false;
		}*/
	}

	void SoldierDoll::Escape() {
		switch (_aState) {
		case ActionState::PATROL:
			if (0 < _patrolX) _direction = enemy::MOVE_LEFT;
			else if (_patrolX < 0) _direction = enemy::MOVE_RIGHT;
			//if (_patrolX != 0) {
			//	switch (_direction) {
			//	case enemy::MOVE_LEFT:

			//		break;
			//	case enemy::MOVE_RIGHT:
			//		break;
			//	}
			//}
			return;

		case ActionState::ESCAPE:
			if (_aCount == 0 && _stand == true) PlaySe(enemy::soldier::ESCAPE_SE);
			if (_actionX < 0) {
				_direction = enemy::MOVE_LEFT;
			}
			else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;


			if (_actionX != 0) {
				switch (_direction) {
				case enemy::MOVE_LEFT:
					_actionX += enemy::ESCAPE_VECTOR / FRAME;
					_moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
					break;
				case enemy::MOVE_RIGHT:
					_actionX -= enemy::ESCAPE_VECTOR / FRAME;
					_moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
					break;
				}
				if (_actionX == 0) { 
					ChangeIdol(IDOL_FRAME);
				}
			}
			return;
		case ActionState::ATTACK:
			if (_aCount == 0) PlaySe(enemy::soldier::ATTACK_SE);
			if (_actionX < 0) {
				_direction = enemy::MOVE_RIGHT;
			}
			else if (0 < _actionX) _direction = enemy::MOVE_LEFT;

			if (_actionX != 0) {
				double mv = 0;
				switch (_direction) {
				case enemy::MOVE_LEFT:
					_actionX -= enemy::ATTACK_VECTOR / FRAME;
					mv = -enemy::ATTACK_VECTOR / FRAME;
					if (_actionX < 0) _actionX = 0;
					break;
				case enemy::MOVE_RIGHT:
					_actionX += enemy::ATTACK_VECTOR / FRAME;
					mv = enemy::ATTACK_VECTOR / FRAME;
					if (0 < _actionX) _actionX = 0;
					break;
				}
				_moveVector.GetPX() = mv;
				if (_actionX == 0) { 
					ChangeIdol(IDOL_FRAME);
				}
			}
			return;
		default:
			return;
		}
	}

	void SoldierDoll::PatrolOn() {
		if (_soul == nullptr) return;
		EnemyBase::PatrolOn();
		StopSound();

		// 魂の色に応じてキーを切り替え
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

		if (_actionX == 0) {
			if(_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}

	void SoldierDoll::AttackOn() {
		if (_aState != ActionState::ATTACK) {
			ChangeState(ActionState::ATTACK, enemy::red::SOLDIER_ATTACK);
			(_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
			_searchBox.SetCollisionFlag() = false;	// アクションに突入したら一時的に索敵処理を切る
		}
	}

	void SoldierDoll::EscapeOn() {

		// 逃走状態ではない場合のみ、
		if (_aState != ActionState::ESCAPE) {
			ChangeState(ActionState::ESCAPE, enemy::blue::SOLDIER_ESCAPE);
			(_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
			_searchBox.SetCollisionFlag() = false;	// アクションに突入したら一時的に索敵処理を切る
		}
	}

	void SoldierDoll::PositionUpdate() {
		// 移動ベクトルYに加速度を代入
		_moveVector.GetPY() = _gravity;
		// マップチップにめり込んでいる場合は座標を修正
		if (_soul != nullptr) {
			int i = 0;
		}
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);
		GimmickCheck(_moveVector);
		_position = _position + _moveVector;	// 位置座標を更新

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		if (_aState == ActionState::ATTACK) {
			auto it = _collisions.find(_divKey.first);
			it->second.Update(_position, _direction);
#ifdef _DEBUG
			if(it->second.SetDrawFlag() == false) it->second.SetDrawFlag() = true;
#endif
		}

		auto col = _collisions.find(enemy::SOLDIER_EMPTY);
		col->second.Update(_position, _direction);

		if (_soul == nullptr && IsAnimationMax() == true) {
			col->second.SetCollisionFlag() = true;
			// _mainCollision.Swap(col->second);
#ifdef _DEBUG
			_searchBox.SetDrawFlag() = false;
#endif
			
		}
	}

	void SoldierDoll::Attack() {
		if (_soul == nullptr || _aState == ActionState::WAKEUP) return;	// 魂がない場合は処理をスキップ
		auto&& player = _game.GetObjectServer()->GetPlayer();
		auto playerBox = player->GetMainCollision();	// プレイヤーの当たり判定を取得

		// ギミック（レバー）と衝突したか？
		if (_aState == ActionState::ATTACK) {
			auto gs = _game.GetGimmickServer()->GetGimmicks();
			for (auto gg : gs) {
				if (gg->GimmickType() != gimmick::LEVER) continue;	// レバーではない場合はスキップ
				if (_mainCollision.HitCheck(gg->GetMainCollision())) {
					std::dynamic_pointer_cast<Lever>(gg)->OpenDoor();
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
		if (_aState == ActionState::ATTACK) {
			auto box = _collisions.find(_divKey.first);
			// 接触している場合は自機の衝突処理を呼び出す
			if (box->second.HitCheck(playerBox)) {
				// 進行方向とは同方向に対象を飛ばす
				player->Damage(SearchPosition());
			}
		}

		//if (_mainCollision.HitCheck(playerBox)) {
		//	player.Da
		//}
	}

	void SoldierDoll::ChangeIdol(int stay) {
		_stay = stay;
		_searchBox.SetCollisionFlag() = false;	// 当たり判定を切る
		std::string nextkey = "";
		switch (_soul->SoulColor()) {
		case soul::RED:
			nextkey = enemy::red::SOLDIER_IDOL;
			break;
		case soul::BLUE:
			nextkey = enemy::blue::SOLDIER_IDOL;
			break;
		}
		ChangeState(ActionState::IDOL, nextkey);
	}

	bool SoldierDoll::Hit() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();

		// プレイヤーは自身に接触しているかどうか？
		if (_mainCollision.HitCheck(player->GetMainCollision())) {
			player->GetMainCollision();
		}
		return false;
	}


	void SoldierDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// 現在の急所がある座標を算出
		auto vitalPart = VitalPart(_mainCollision, SOLDIER_VITAL);
		auto player = _game.GetObjectServer()->GetPlayer();
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
					StopSound();
					PlaySe(enemy::soldier::DOWN);
					_searchBox.SetCollisionFlag() = false;	// 一時的に索敵判定を切る
					_mainCollision.SetCollisionFlag() = false;
					_soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
					_stay = 0;
					_isAction = false;

					auto hiteff = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy:: S_HIT, _position, 30);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hiteff), effect::type::FORMER);

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
			if(player->HaveSoul()){
				// 魂が空の場合にボックスが接触したら
				if (_soul == nullptr) {
					// 接触時の判定はAABBで行う（奪うアクションとは違い、向きによる制限なし）
					if (NowCollision(_divKey.first).HitCheck(acollision)) {
						// プレイヤーを取得
						auto player = _game.GetObjectServer()->GetPlayer();
						_soul = player->GiveSoul();	// プレイヤ―から対象の魂を受け取る
						_soul->Inactive();	// 魂を非活性状態にする
						PlaySe(key::SOUND_PLAYER_GIVE_TRUE);
						_searchBox.SetCollisionFlag() = false;
						switch (_soul->SoulColor()) {
						case soul::RED:
							ChangeState(ActionState::WAKEUP, enemy::red::SOLDIER_WAKEUP);
							return;
						case soul::BLUE:
							ChangeState(ActionState::WAKEUP, enemy::blue::SOLDIER_WAKEUP);
							return;
						}
						return;
					}
				}
			}
		}
	}

	void SoldierDoll::SetParameter(ObjectValue objValue) {
		EnemyBase::SetParameter(objValue);
		if (_oValue.SoulType() == 0) {	// 魂が空の場合は抜け殻になる
			auto embox = _collisions.find(enemy::SOLDIER_EMPTY);
			embox->second.Update(_position, _direction);
			ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
			_aCount = AnimationCountMax();	// カウンタをマックスにする
			_searchBox.SetCollisionFlag() = false;	// 一時的に索敵判定を切る
			_mainCollision.SetCollisionFlag() = false;
			_changeGraph = false;
			return;	// 処理を抜ける
		}
		auto soul_n = std::make_shared<SoulSkin>(_game.GetGame());
		// auto sn = std::static_pointer_cast<SoulSkin>(soul_n);
		switch (_oValue.SoulType()) {
		case 1:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::red::SOLDIER_PATROL);
			break;
		case 2:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::blue::SOLDIER_PATROL);
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

	void SoldierDoll::Death() {
		ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
		EnemyBase::Death();
		PlaySe(enemy::soldier::DOWN);
	}

	AABB SoldierDoll::NowCollision(std::string key) {
		if(_soul != nullptr) return _mainCollision;
		auto it = _collisions.find(enemy::SOLDIER_EMPTY);
		// 現在のアクション状態はボックスを修正する必要があるか？
		return it->second;
	}

	bool SoldierDoll::StopSound() {
		auto attack = se::SoundServer::GetSound(enemy::soldier::ATTACK_SE);
		auto escape = se::SoundServer::GetSound(enemy::soldier::ESCAPE_SE);
		if (CheckSoundMem(attack) == 1) StopSoundMem(attack);
		if (CheckSoundMem(escape) == 1) StopSoundMem(escape);
		return true;
	}
}