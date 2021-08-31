#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "Player.h"
#include "ResourceServer.h"

// α仮
#include "SoulSkin.h"
#include <memory>

#ifdef _DEBUG
namespace {
	constexpr auto START_POS_X = 500;
	constexpr auto START_POS_Y = 0;

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
	constexpr auto SEARCH_X = 100 + (SOLDIER_BOX_W * 2);
	constexpr auto SEARCH_Y1 = SOLDIER_BOX_H1;
	constexpr auto SEARCH_Y2 = SOLDIER_BOX_H2;

	// 巡回範囲(最大Vector)
	constexpr auto PATROL_VECTOR = 1.0;
	constexpr auto PATROL_MAX = 280;

	constexpr auto FRAME = 30;

	// アイドルモーション時間
	constexpr auto STAY_MAX = 30;	//　stay
}
#endif
namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::SOLDIER_EMPTY, key::SOUND_NUM);
		_position = { START_POS_X, START_POS_Y };

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
		_mainCollision = { _position, SOLDIER_BOX_W, SOLDIER_BOX_W, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_searchBox = { _position, SEARCH_X, SEARCH_X, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };

		/*_searchBox = { { _mainCollision.GetMin().GetX() - SEARCH_X, _mainCollision.GetMin().GetY() - SEARCH_Y }, {
						_mainCollision.GetMax().GetX() + SEARCH_X, _mainCollision.GetMax().GetY() + SEARCH_Y } };*/
		// 各種判定の格納（攻撃判定・）
		_collisions = {
			{ enemy::SOLDIER_EMPTY, { _position, 65, 60, -45, 80 , true}},	// 抜け殻時の情報
		};

		_motionKey =
		{	{ enemy::SOLDIER_EMPTY, {40, 0}},

			{ enemy::red::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::red::SOLDIER_IDOL, {28, 0}},
			{ enemy::red::SOLDIER_PATROL, {36, 0}},
			{ enemy::red::SOLDIER_ATTACK, {12, 50}},	// SE有り

			{ enemy::blue::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::blue::SOLDIER_IDOL, {28, 0}},
			{ enemy::blue::SOLDIER_PATROL, {42, 0}},
			{ enemy::blue::SOLDIER_ESCAPE, {24, 0}}
		};

		PatrolOn();
		_stay = 0;
	}

	void SoldierDoll::Process() {
		ObjectBase::Process();
		_moveVector.GetPX() = 0;

		// アニメーションが終わっていない場合はカウントを増やす
		AnimationCount();

		Patrol();
		Action();

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

	void SoldierDoll::Patrol() {
		if (_soul == nullptr) return;	// 魂が空の場合は処理を行わない
		if (_stay != 0) {
			--_stay;	// 待機モーション中はカウンタを減らして処理を抜ける
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
		if (_moveVector.GetX() < 0) {
			_direction = true;
		}
		else if (0 < _moveVector.GetX()) {
			_direction = false;
		}

		if (_actionX < 0) { 
			_direction = enemy::MOVE_LEFT; }
		else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;

		// 索敵範囲の移動距離は280pixel
		if (_actionX != 0) {
			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX += enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
				break;
			case enemy::MOVE_RIGHT:
				_actionX -= enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
			}
		}
	}

	void SoldierDoll::Action() {
		// プレイヤーを発見できるか
		if (SearchPlayer() == true) {
			// 入っている魂に応じて処理を変更する
			switch (_soul->SoulColor()) {
			// 赤い魂の時は、突進処理を実行する。
			case soul::RED:
				// Rush();
				break;
			case soul::BLUE:
				EscapeOn();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			}
		}
		// 発見できなかった場合は移動処理を行う
		if (_soul == nullptr) _actionX = 0;
	}

	void SoldierDoll::PatrolOn() {
		if (_soul == nullptr) return;
		_changeGraph = true;
		_aState = ActionState::PATROL;

		// 魂の色に応じてキーを切り替え
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

		if (_actionX == 0) {
			if(_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}

	void SoldierDoll::PositionUpdate() {
		// 移動ベクトルYに加速度を代入
		_moveVector.GetPY() = _gravity;
		// マップチップにめり込んでいる場合は座標を修正
		_game.GetMapChips()->IsHit(_mainCollision, _position,_moveVector, _direction);
		_position = _position + _moveVector;	// 位置座標を更新

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		if (_soul == nullptr && IsAnimationMax() == true) {
			auto col = _collisions.find(enemy::SOLDIER_EMPTY);
			col->second.Update(_position, _direction);
			_mainCollision.Swap(col->second);
			_searchBox.GetbDrawFlg() = false;
			
		}
	}

	void SoldierDoll::Attack() {
		auto&& player = _game.GetObjectServer()->GetPlayer();	//　プレイヤーの参照を取得
		auto playerBox = player->GetMainCollision();

		//if (_mainCollision.HitCheck(playerBox)) {
		//	player.Da
		//}
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
		auto vitalPart = VitalPart(_mainCollision);
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);

					_soul->SetSpwan(_position);	// 自身の中心座標に実体化させる

					auto player = _game.GetObjectServer()->GetPlayer();

					// 自機が保有する魂が所持上限に到達している場合は所有権を手放す
					if (player->IsSoulMax()) {
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
			// 魂が空の場合にボックスが接触したら
			if (_soul== nullptr) {
				// 接触時の判定はAABBで行う（奪うアクションとは違い、向きによる制限なし）
				if (_mainCollision.HitCheck(acollision)) {
					// プレイヤーを取得
					auto player = _game.GetObjectServer()->GetPlayer();
					_soul = player->GiveSoul();	// プレイヤ―から対象の魂を受け取る
					_soul->Inactive();	// 魂を非活性状態にする
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

	AABB SoldierDoll::VitalPart(Collision& col) {
		// 座標を算出（y座標は変更ない）
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		if (_direction) {
			vitalMin.GetPX() = col.GetMax().GetX() - SOLDIER_VITAL;
			vitalMax.GetPX() = col.GetMax().GetX();
		} else {
			// 右に向いている場合
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + SOLDIER_VITAL;
		}
		return AABB(vitalMin, vitalMax, true);
	}
}