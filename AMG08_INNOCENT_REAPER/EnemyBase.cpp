#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"

using std::string;

namespace {
	constexpr auto FRAME = 30;
}

namespace inr {


	EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_sState = SoulState::EMPTY;
		_divKey = std::make_pair("NULL", key::SOUND_NUM);
		_position = { 0, 0 };
		_moveVector = { 0, 0 };
		_actionX = 0;

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_direction = true;
		_changeGraph = true;
		Init();
	}

	EnemyBase::~EnemyBase() {

	}

	void EnemyBase::Init() {
		// _collisions = { {enemy::ENEMY_EMPTY, {Vector2(), Vector2()}} };
	}

	void EnemyBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
		//// アニメーションが終わっていない場合はカウントを増やす
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// カウンター初期化
		DrawDebugBox(_mainCollision);
		DrawDebugBox(_searchBox);
	}

	/*void EnemyBase::AnimationInit() {

	}

	void EnemyBase::Move() {

	}

	void EnemyBase::Action() {

	}*/

	bool EnemyBase::SearchPlayer() {
		// 魂が空ではない時
		if (_sState != SoulState::EMPTY) {
			// プレイヤークラスの参照からメインの当たり判定を取得
			auto player = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);
			const auto& pbox = player.GetMainCollision();
			// 索敵範囲と自機は当たっているか？
			if (_searchBox.HitCheck(pbox)) {
				// 自機は自分と同じ方向にいるか？
				if (_direction == SearchPosition()) {
					// 発見
					return true;
				}
			}
		}
		return false;
	}

	bool EnemyBase::SearchPosition() {
		auto pl = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);
		// プレイヤーのx座標を取得
		auto px = pl.GetPosition().GetX();
		// Vector 
		auto myx = _position.GetX();
		// Enemyのx座標からplayerのx座標を引く
		// その値が+の場合はプレイヤーのx座標が敵よりも小さい(左側にいる)
		// その逆で値が-の場合は右側にいる
		auto way = myx - px;
		// 値が正の数の場合（右側にいる）
		if (way < 0) return true;
		// 値が負の数の場合（左側にいる）
		return false;
	}

	void EnemyBase::Escape() {
		// 状態が逃走ではない場合
		if (_aState != ActionState::ESCAPE) {
			_changeGraph = true;
			_aState = ActionState::ESCAPE;
			_divKey.first = enemy::blue::SOLDIER_ESCAPE;
			// 自身の向きによって移動距離を変更
			if (_direction) _actionX = -enemy::ESCAPE_MAX / FRAME;
			else _actionX = enemy::ESCAPE_MAX / FRAME;
		}
		//// 向きによって加算・減算切り替え
		//if (_direction) {
		//	_actionX -= ESCAPE_VECTOR / FRAME;
		//	_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
		//	return;
		//}
		//_actionX += ESCAPE_VECTOR / FRAME;
		//_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
	}

	bool EnemyBase::Hit() {
		return false;
	}
}