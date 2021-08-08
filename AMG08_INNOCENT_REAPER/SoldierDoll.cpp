#include "SoldierDoll.h"
#include "Game.h"
#include "MapChips.h"

#ifdef _DEBUG
namespace {
	constexpr auto START_POS_X = 300;
	constexpr auto START_POS_Y = 0;

	// 幅・高さ
	constexpr auto SOLDIER_W = 100;
	constexpr auto SOLDIER_H = 240;

	// 当たり判定
	constexpr auto COLLISION_W = 60;

	constexpr auto EMPTY_W = 250;
	constexpr auto EMPTY_H = 100;

	// 索敵ボックス
	constexpr auto SEARCH_X = 100;
	constexpr auto SEARCH_Y = 25;

	// 巡回範囲(最大Vector)
	constexpr auto PATROL_VECTOR = 1.0;
	constexpr auto PATROL_MAX = 100;
}
#endif
namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::PATROL;
		_sState = SoulState::BLUE;
		_divKey = std::make_pair(enemy::SOLDIER_PATROL, key::SOUND_NUM);
		_position = { START_POS_X, START_POS_Y };

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_patrolX = 0;
		_direction = true;
		_changeGraph = true;
		Init();
	}

	SoldierDoll::~SoldierDoll() {
	}

	void SoldierDoll::Init() {
		// メインの当たり判定を初期化
		_mainCollision = { _position, SOLDIER_W / 2, SOLDIER_H / 2 };
		_searchBox = { _position, SEARCH_X, SOLDIER_H / 2 };

		/*_searchBox = { { _mainCollision.GetMin().GetX() - SEARCH_X, _mainCollision.GetMin().GetY() - SEARCH_Y }, {
						_mainCollision.GetMax().GetX() + SEARCH_X, _mainCollision.GetMax().GetY() + SEARCH_Y } };*/
		// 各種判定の格納（攻撃判定・）
		_collisions = {
			// 抜け殻時の情報
			{ enemy::SOLDIER_EMPTY, { _position, EMPTY_W / 2, EMPTY_H / 2 }},
		};

		_enemyKey = { { enemy::SOLDIER_EMPTY,{ 121, 50}},
			{enemy::SOLDIER_PATROL, {61, 50}},
			{enemy::SOLDIER_ATTACK, {22, 50}},
			{enemy::SOLDIER_ESCAPE, {29, 50}} };

		PatrolOn();
	}

	void SoldierDoll::Process() {
		ObjectBase::Process();

		// アニメーションが終わっていない場合はカウントを増やす
		if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		else _aCount = 0;	// カウンター初期化

		Patrol();
		Action();

		// 当たり判定を取得

		Move();
	}

	void SoldierDoll::Patrol() {
		if (_aState != ActionState::PATROL && _aState != ActionState::ESCAPE) {
			PatrolOn();
		}
		if (_aState == ActionState::PATROL) {
			// 左移動
			if (_patrolX < 0) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				return;
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
			}

			if (static_cast<int>(_patrolX) == 0) {
				if (_direction) _patrolX = -PATROL_MAX;
				else _patrolX = PATROL_MAX;
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

		if (_game.GetMapChips()->IsHitExt(_mainCollision, _moveVector)) {
			if (0 < _gravity) {
				_stand = true;
			}
			_gravity = 0;
		}

		_moveVector.GetPY() = _gravity;
		_position = _position + _moveVector;
		_mainCollision.Updata(_position, _direction);
		_searchBox.Updata(_position, _direction);
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
				Escape();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			default:
				break;
			}
		}
		// 発見できなかった場合は移動処理を行う
	}

	void SoldierDoll::PatrolOn() {
		_changeGraph = true;
		_aState = ActionState::PATROL;
		_divKey.first = enemy::SOLDIER_PATROL;

		if (_actionX <= 0) {
			if(_direction) _patrolX = PATROL_MAX;
			else _patrolX = -PATROL_MAX;
		}
	}

	/*void SoldierDoll::AnimationInit() {

	}*/
}