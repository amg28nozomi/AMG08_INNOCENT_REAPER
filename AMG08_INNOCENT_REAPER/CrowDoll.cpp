#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "SoulSkin.h"

#include <random>
#include <time.h>
#include <memory>

namespace {
	constexpr auto CROW_WIDTH = 60;	// 横の当たり判定
	constexpr auto CROW_HEIGHT = 180;	// 縦の当たり判定

	constexpr auto CROW_VITAL = 20;
	constexpr auto CROW_LIFE_MAX = 10;
	constexpr auto CROW_ANGER = 5;	// 怒り状態になるか
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::CROW_DOLL;
		Init();
	}

	void CrowDoll::Init() {
		_target = { 0, 0 };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// 当たり判定
		_collisions = {
			{enemy::crowdoll::CROW_RASH, {_position, 0, 120, 130 ,CROW_HEIGHT / 2, true}},	// 連撃攻撃の当たり判定
		//	{enemy::crowdoll::CROW_BLINK, {_position, }}
		};
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {25, 0}},
			{enemy::crowdoll::CROW_RASH , {40, 20}},
			{enemy::crowdoll::CROW_BLINK , {25, 20}},
			{enemy::crowdoll::CROW_GROWARM , {25, 20}},
			{enemy::crowdoll::CROW_ROAR , {25, 50}},
			{enemy::crowdoll::CROW_DEBUF, {30, 50}},
			{enemy::crowdoll::CROW_DOWN , {25, 50}},
		};

	}

	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions().at(0);	// 座標を更新
		Init();
	}

	void CrowDoll::Process() {

	}

	void CrowDoll::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
#endif
	}

	void CrowDoll::WakeUp() {
		// 起き上がりモーション

	}

	void CrowDoll::GetTarget() {
		_target = _game.GetObjectServer()->GetPlayer()->GetPosition();	// 目標の現在地点を取得する
	}

	void CrowDoll::ChangeDirection() {
		// 移動方向に応じて向きを変更する
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}

	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;
		_cState = nextState;
		_changeGraph = true;
	}

	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_cState == CrowState::DEATH) return;	// 死んでいる場合は魂を奪えない
		if (IsVital() != true) return;	// 隙がない場合も魂を奪えない
		if (ckey == PKEY_ROB) {	// 魂を奪いにきているか？
			auto vitalPart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// 怯み状態にする
					AddSoul();	// 魂を生み出す
					// ここで死亡処理を行うか判定を行う
					return;
			}
		}
	}

	bool CrowDoll::IsVital() {
		switch (_cState) {
		case CrowState::IDOL:
		case CrowState::DEBUF:
		case CrowState::ROAR:
		case CrowState::WINCE:
		case CrowState::SLEEP:
				return false;
		case CrowState::RASH:
			return true;
		default:
			return false;
		}
	}

	void CrowDoll::AddSoul() {
		auto player = _game.GetObjectServer()->GetPlayer();	// 自機の情報を取得する
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());	// 魂を生成する
		srand((unsigned int)time(NULL));	// 乱数初期化
		auto type = rand() % 2;
		switch (type) {
		case 0:
			soul->SetParameter(1, 7);	// 赤の魂
			break;
		case 1:
			soul->SetParameter(2, 7);	// 青の魂
			break;
		default:
			break;
		}
		soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
		// 自機は魂の所持上限に到達しているか？
		if (player->IsSoulMax()) {
			_soul->OwnerNull();	// 所有者はいない
			_game.GetObjectServer()->Add(std::move(soul));	// オブジェクトサーバーに登録する
			return;
		}
		player->SoulCatch(std::move(_soul));	// そうではない場合は魂の所有権をプレイヤーに譲渡
	}

	bool CrowDoll::IsAnger() {
		if (CROW_ANGER < _life) return false;	// 怒り状態になっていない
		return true;
	}

	AABB CrowDoll::NowCollision(std::string key) {
		return _mainCollision;
	}
}
