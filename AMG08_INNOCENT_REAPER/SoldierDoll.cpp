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
		_divKey = std::make_pair(enemy::blue::SOLDIER_PATROL, key::SOUND_NUM);
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

		_enemyKey =
		{	{ enemy::SOLDIER_EMPTY, {18, 0}},

			{ enemy::red::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::red::SOLDIER_IDOL, {32, 0}},
			{ enemy::red::SOLDIER_PATROL, {36, 0}},
			{ enemy::red::SOLDIER_ATTACK, {12, 50}},	// SE有り

			{ enemy::blue::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::blue::SOLDIER_IDOL, {28, 0}},
			{ enemy::blue::SOLDIER_PATROL, {28, 0}},
			{ enemy::blue::SOLDIER_ESCAPE, {24, 0}}
		};

		PatrolOn();
	}

	void SoldierDoll::Process() {
		ObjectBase::Process();

		// アニメーションが終わっていない場合はカウントを増やす
		if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		else _aCount = 0;	// カウンター初期化

		//Patrol();
		//Action();

		//// 当たり判定を取得

		//Move();
		//PositionUpdate();
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
		_game.GetMapChips()->IsHit(_mainCollision, _position,_moveVector);
		_position = _position + _moveVector;	// 位置座標を更新

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);
	}
}