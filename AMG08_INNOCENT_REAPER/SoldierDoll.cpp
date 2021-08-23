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
}
#endif
namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::IDOL;
		_sState = SoulState::BLUE;
		_divKey = std::make_pair(enemy::blue::SOLDIER_IDOL, key::SOUND_NUM);
		_position = { START_POS_X, START_POS_Y };

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_patrolX = 0;
		_aInterval = 0;
		_direction = true;
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
			{ enemy::red::SOLDIER_IDOL, {32, 0}},
			{ enemy::red::SOLDIER_PATROL, {36, 0}},
			{ enemy::red::SOLDIER_ATTACK, {12, 50}},	// SE有り

			{ enemy::blue::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::blue::SOLDIER_IDOL, {28, 0}},
			{ enemy::blue::SOLDIER_PATROL, {42, 0}},
			{ enemy::blue::SOLDIER_ESCAPE, {24, 0}}
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

		// 当たり判定を取得

		Move();
		PositionUpdate();
	}

	void SoldierDoll::StateUpdate() {
		switch (_sState) {
		// 魂がない場合は何もしないよー
		case SoulState::EMPTY:
			// 脱出
			break;
		// 青い魂の場合
		case SoulState::BLUE:
			// 巡回(索敵)→逃走→待機
			// インターバルがある場合はintervalを減少させて処理を抜ける
			if (_aInterval) { 
				--_aInterval; 
				break;
			}
			// 索敵を行う

			break;
		// 赤い魂の場合
		case SoulState::RED:
			if (_aInterval) {
				--_aInterval;
				break;
			}

			break;
		// 魂が空の時の処理
		/*case ActionState::EMPTY:
			break;
		case ActionState::IDOL:
			break;
		case ActionState::PATROL:
			break;
		case ActionState::ATTACK:
			break;
		case ActionState::ESCAPE:
			break;
		default:
			break;*/
		}
	}

	void SoldierDoll::Patrol() {
		// 魂が空の場合はゼロにする
		if (_aState == ActionState::EMPTY) _patrolX = 0;
		else if (_aState != ActionState::PATROL && _aState != ActionState::ESCAPE && _aState!= ActionState::IDOL) {
			PatrolOn();
		}
		if (_aState == ActionState::PATROL) {
			// 左移動
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				_moving = enemy::MOVE_LEFT;
				return;
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				_moving = enemy::MOVE_RIGHT;
			}

			if (static_cast<int>(_patrolX) == 0) {
				if (_moving == enemy::MOVE_LEFT) _patrolX = PATROL_MAX;
				else _patrolX = -PATROL_MAX;
			}
		}
	}

	void SoldierDoll::Move() {
		// 移動ベクトルに応じて、向きを変更
		// 移動量に応じて向きを変更
		if (_moveVector.GetX() < 0) {
			_direction = false;
		}
		else if (0 < _moveVector.GetX()) {
			_direction = true;
		}

		// 索敵範囲の移動距離は280pixel
		if (_actionX != 0) {
			switch (_moving) {
			case enemy::MOVE_LEFT:
				_actionX -= enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
				break;
			case enemy::MOVE_RIGHT:
				_actionX += enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
			}

		}
	}

	void SoldierDoll::Action() {
		// プレイヤーを発見できるか
		if (SearchPlayer() == true) {
			// 入っている魂に応じて処理を変更する
			switch (_sState) {
			// 赤い魂の時は、突進処理を実行する。
			case SoulState::RED:
				// Rush();
				break;
			case SoulState::BLUE:
				EscapeOn();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			default:
				break;
			}
		}
		// 発見できなかった場合は移動処理を行う
		if (_sState == SoulState::EMPTY) _actionX = 0;
	}

	void SoldierDoll::PatrolOn() {
		_changeGraph = true;
		_aState = ActionState::PATROL;

		// 魂の色に応じてキーを切り替え
		(_sState == SoulState::BLUE) ? 
			_divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

		if (_actionX <= 0) {
			if(_direction) _patrolX = PATROL_MAX;
			else _patrolX = -PATROL_MAX;
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

		if (_sState == SoulState::EMPTY && IsAnimationMax() == true) {
			auto col = _collisions.find(enemy::SOLDIER_EMPTY);
			col->second.Update(_position, _direction);
			_mainCollision.Swap(col->second);
			_searchBox.GetbDrawFlg() = false;
			
		}
	}

	bool SoldierDoll::Hit() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);

		// プレイヤーは自身に接触しているかどうか？
		if (_mainCollision.HitCheck(player.GetMainCollision())) {
			player.GetMainCollision();
		}
		return false;
	}

	void SoldierDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// 現在の急所がある座標を算出
		auto vitalPart = VitalPart(_mainCollision);
		// 魂は奪われるか？
		if (ckey == PKEY_ROB) {
			if (_sState != SoulState::EMPTY) {
				if (_direction != direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					std::string scolor;
					double sp;
					if (_sState == SoulState::BLUE) {
						scolor = "blue";
						sp = 7;
					}
					else if (_sState == SoulState::RED) {
						scolor = "red";
						sp = 7.5;
					}

					_divKey.first = enemy::SOLDIER_EMPTY;
					_aState = ActionState::EMPTY;
					_sState = SoulState::EMPTY;
					_changeGraph = true;

					auto bSoul = std::make_unique<SoulSkin>(_game.GetGame());

					bSoul->SetStatus(_position, scolor);
					bSoul->SetSpeed(sp);
					_game.GetObjectServer()->Add(std::move(bSoul));
				}
			}
		}
	}

	AABB SoldierDoll::VitalPart(Collision& col) {
		// 座標を算出（y座標は変更ない）
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		if (_direction) {
			// 右に向いている場合
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + SOLDIER_VITAL;
		} else {
			vitalMin.GetPX() = col.GetMax().GetX() - SOLDIER_VITAL;
			vitalMax.GetPX() = col.GetMax().GetX();
		}
		return AABB(vitalMin, vitalMax, true);
	}
}