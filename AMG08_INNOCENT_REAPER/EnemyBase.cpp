#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"

#include "SoulSkin.h"

using std::string;

namespace {
	constexpr auto FRAME = 30;
}

namespace inr {


	EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair("NULL", key::SOUND_NUM);
		_position = { 0, 0 };
		_moveVector = { 0, 0 };
		_actionX = 0;
		_patrolX = 0;
		_aInterval = 0;

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_stay = 0;
		_direction = false;
		_changeGraph = true;
		_drawStop = false;

		_soul = nullptr;

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

		/*std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}*/
		//// アニメーションが終わっていない場合はカウントを増やす
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// カウンター初期化
		/*DrawDebugBox(_mainCollision);*/
		DrawDebugBox(_searchBox);

		/*auto it = _collisions.find(_divKey.first);
		if (it == _collisions.end()) return;
		DrawDebugBox(it->second);*/
	}

	void EnemyBase::AnimationCount() {
		if (_soul == nullptr && IsAnimationMax() == true) return;
		ObjectBase::AnimationCount();
	}

	/*void EnemyBase::AnimationInit() {

	}

	void EnemyBase::Move() {

	}

	void EnemyBase::Action() {

	}*/

	bool EnemyBase::SearchPlayer() {
		// 魂が空ではない時
		if (_soul != nullptr) {
			// プレイヤークラスの参照からメインの当たり判定を取得
			auto player = _game.GetObjectServer()->GetPlayer();
			const auto& pbox = player->GetMainCollision();
			// 索敵範囲と自機は当たっているか？
			if (_searchBox.HitCheck(pbox)) {
				// 自機は自分と同じ方向にいるか？
				if (_direction == SearchPosition()) {
					// 発見
					return true;
				}
			}
		}
		// 当たり判定が無かった
		return false;
	}

	bool EnemyBase::SearchPosition() {
		auto pl = _game.GetObjectServer()->GetPlayer();
		// プレイヤーのx座標を取得
		auto px = pl->GetPosition().GetX();
		// Vector 
		auto myx = _position.GetX();
		// Enemyのx座標からplayerのx座標を引く
		// その値が+の場合はプレイヤーのx座標が敵よりも小さい(左側にいる)
		// その逆で値が-の場合は右側にいる
		auto way = myx - px;
		// 値が正の数の場合（右側にいる）
		if (way < 0) return false;
		// 値が負の数の場合（左側にいる）
		return true;
	}

	void EnemyBase::PositionUpdate() {

	}

	void EnemyBase::EscapeOn() {
	}

	void EnemyBase::PatrolOn() {
		_searchBox.GetCollisionFlgB() = true;
		_changeGraph = true;
		_aState = ActionState::PATROL;
	}

	void EnemyBase::AttackOn() {

	}

	void EnemyBase::ChangeState(ActionState nextstate, std::string key) {
		_changeGraph = true;
		_aState = nextstate;
		_divKey.first = key;
	}

	void EnemyBase::ChangeIdol() {

	}

	bool EnemyBase::Hit() {
		return false;
	}

	void EnemyBase::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];	// 座標更新
		_mainCollision.Update(_position, _direction);	// 当たり判定修正
		_searchBox.Update(_position, _direction);	// 索敵範囲の修正
	}

	void EnemyBase::CollisionHit(const std::string ckey, Collision acollision, bool direction) {

	}

	bool EnemyBase::IsEmpty() {
		if (_soul == nullptr) return true;
		return false;
	}

	void EnemyBase::Death() {
		_soul->SetSpwan(_position);	// 自身の座標に魂を実体化する
		_soul->OwnerNull();
		_soul.reset();	// 魂の所有権を手放す
	}

	void EnemyBase::Action() {
		// プレイヤーを発見できるか
		if (SearchPlayer() == true) {
			// 入っている魂に応じて処理を変更する
			switch (_soul->SoulColor()) {
				// 赤い魂の時は、突進処理を実行する。
			case soul::RED:
				AttackOn();
				if (_actionX == 0) {
					PatrolOn();
				}
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

	AABB EnemyBase::VitalPart(Collision& col, int vital) {
		// 座標を算出（y座標は変更ない）
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		if (_direction) {
			vitalMin.GetPX() = col.GetMax().GetX() - vital;
			vitalMax.GetPX() = col.GetMax().GetX();
		}
		else {
			// 右に向いている場合
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + vital;
		}
		return AABB(vitalMin, vitalMax, true);
	}

	AABB EnemyBase::DamageBox(int fix) {
		// ベクトル作成
		auto damageMin(_mainCollision.GetMin());
		auto damageMax(_mainCollision.GetMax());
		if (_direction) {
			damageMax.GetPX() -= fix;
		}
		else {
			damageMin.GetPX() += fix;
		}
		return AABB(damageMin, damageMax, true);
	}

	bool EnemyBase::IsStandChip() {
		auto nowcol = NowCollision(_divKey.first);
		auto chipType = _game.GetMapChips()->IsStand(nowcol, _position, _gravity, &_lastChip);
		switch (chipType) {
		case mapchip::IVY:
		case mapchip::NONE:
			return false;
		case mapchip::THORM:
			if (_soul != nullptr) Death();
		default:
			return true;
		}
	}
}