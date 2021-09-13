#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "MapChips.h"
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

	constexpr auto FLAOT_MAX = 200;
	constexpr auto DEFAULT_Y = 870 - 300;

	constexpr auto IS_ANGER = 1;
	constexpr auto IS_NORMAL = 0;

	constexpr auto RASH_MAX = 150;
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;
		_aState = ActionState::IDOL;
		_eType = EnemyType::CROW_DOLL;
		Init();
	}

	void CrowDoll::Init() {
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// 当たり判定
		_collisions = {
			{enemy::crowdoll::CROW_RASH, {_position, 0, 120, 130 , CROW_HEIGHT / 2, true}},	// 連撃攻撃の当たり判定
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {25, 0}},
			{enemy::crowdoll::CROW_RASH , {40, 20}},
			{enemy::crowdoll::CROW_BLINK , {25, 20}},
			{enemy::crowdoll::CROW_GROWARM , {25, 20}},
			{enemy::crowdoll::CROW_ROAR , {25, 50}},
			{enemy::crowdoll::CROW_DEBUF, {30, 50}},
			{enemy::crowdoll::CROW_DOWN , {25, 50}},
			{enemy::crowdoll::CROW_WINCE, {25, 50}},
		};
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_rashCount = 0;
		_setup = false;
		_changeGraph = false;
	}

	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions().at(0);	// 座標を更新
		Init();
	}

	// 攻撃パターンは「ラッシュ→落下→空中待機→腕刺し→待機→空中待機」

	void CrowDoll::Process() {
		IsGravity();
		// バトル開始前の処理
		IsBattle();
		WakeUp();	// 起き上がり
		if (IsActive() != true) return;	// 活動状態でない場合は処理を行わない
		SetState();
		Floating();
		Move();
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
		if (_game.GetModeServer()->GetModeMain()->BossFlag() != true) return;
		if (_setup == true) return;
		--_aCount;	// 立ち上がらせる
		if (_aCount == 0) {
			_setup = true;	// セットアップ完了
			auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
			ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
			return;
		}
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

	void CrowDoll::Move() {
		ChangeDirection();	// 向きの調整
		_moveVector.GetPY() = _gravity;
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);	// 押し出し処理

		_position = _position + _moveVector;
		_mainCollision.Update(_position, _direction);

		_moveVector = { 0, 0 };
	}

	bool CrowDoll::IsGravity() {
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::RASH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
			_gravity += FRAME_G;	// 加速度を加算
			if (MAX_G < _gravity) _gravity = MAX_G;
			if (IsStandChip()) {
				if (0 < _gravity) _stand = true;
				_gravity = 0;
			} else {
				_stand = false;
			}
			break;
		default:
			if (0 < _gravity) _gravity = 0;
			return false;	// 重力処理は行わない
		}
	}

	bool CrowDoll::Floating() {
		if (_cState != CrowState::IDOL) return false;
		if (_position.GetY() == DEFAULT_Y) return false;
		_gravity -= 0.25;
		if (_position.GetY() + _gravity < DEFAULT_Y) { 
			_position.GetPY() = DEFAULT_Y;
			_gravity = 0;
		}
		return true;
	}

	void CrowDoll::Warp() {
		auto p = _position.GetX() - _target.GetX();	// 自機はどちら側にいるか？
		if (p < 0) {	// 自機は右に居る
			_direction = enemy::MOVE_RIGHT;
			auto px = (_target.GetX() - (_mainCollision.GetWidthMin()) * 3);
			_position = { px, 870 };
			_actionEnd.GetPX() = px - RASH_MAX;
		}// 自機は右側にいる
		else if (0 < p) { 
			_direction = enemy::MOVE_LEFT;
			auto px = (_target.GetX() + (_mainCollision.GetWidthMax()) * 3);
			_position = { px, 870 };
			_actionEnd.GetPX() = px + RASH_MAX;
		}// 自機は左側にいる
		_atkInterval = 10;	// 10フレーム後にアクションを実行する
		auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_RASH);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(enemy::crowdoll::SE_RASH));
	}

	void CrowDoll::Rash() {
		// 連続攻撃
		double mx = 0;
		double nextpos = 0;
		switch (_direction) {
		case enemy::MOVE_LEFT:
			mx = -RASH_MAX / 15;
			_moveVector.GetPX() = mx;
			nextpos = _position.GetX() + mx;
			if (IsAttackEnd() == true) {
				--_rashCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RASH));
				return;
			}
			if (_actionEnd.GetX() < nextpos) return;
			_actionEnd.GetPX() = nextpos - RASH_MAX;
			break;
		case enemy::MOVE_RIGHT:
			mx = RASH_MAX / 15;
			_moveVector.GetPX() = mx;
			nextpos = _position.GetX() + mx;
			if (IsAttackEnd() == true) {
				--_rashCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RASH));
				return;
			}
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RASH_MAX;
			break;
		}
		--_rashCount;
		_atkInterval = 20;
	}

	void CrowDoll::Debuf() {
		// 自機に対して弱体効果を付与する
		_game.GetObjectServer()->GetPlayer()->Debuf();
		// デバフエフェクトの生成
		// auto debufEff = std::make_unique<EffectBase>(_game.GetGame(), )
	}

	bool CrowDoll::IsAttackEnd() {
		if (_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction) == mapchip::NORMAL) return true;
		auto moveout = _position + _moveVector;
		return false;
	}

	bool CrowDoll::SetState() {
		// 状態に応じた処理を行う
		switch (_cState) {
		case CrowState::IDOL:	// 空中待機の場合
			// インタール明けに次のアクションを実行する
			if (_atkInterval == 0) {
				ModeChange(CrowState::RASH, enemy::crowdoll::CROW_RASH);	// 状態切り替え
				GetTarget();	// 自機の現在座標を取得する
				Warp();	// 自機の前に跳ぶ
				_rashCount = 3;
			}
			break;
		case CrowState::RASH:
			if (_atkInterval == 0) {
				if (0 < _rashCount) {
					Rash();	// ラッシュアクション実行
					break;
				}
				_actionEnd.GetPX() = 0;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 20;
				// 次の状態に遷移する
			} 
		}
		return true;
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

	bool CrowDoll::IsActive() {
		if (_setup != true) return false;
		if (_cState == CrowState::DEATH || _cState == CrowState::SLEEP) {
			if (IsAnimationMax() != true) ObjectBase::AnimationCount();
			return false;
		}
		// 待ち時間の場合は
		if (0 < _atkInterval) --_atkInterval;	// 待ち時間を減らす
		else ObjectBase::AnimationCount();	// カウントを増やす
		return true;
	}

	bool CrowDoll::IsBattle() {
		if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return false;
		if (_game.GetObjectServer()->GetPlayer()->GetPosition().GetX() < _game.GetMapChips()->GetMapSizeWidth() - HALF_WINDOW_W) return false;
		_game.GetModeServer()->GetModeMain()->BossBattle();	// ボスバトルを開始する
	}

	void CrowDoll::AddSoul() {
		auto player = _game.GetObjectServer()->GetPlayer();	// 自機の情報を取得する
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());	// 魂を生成する
		srand((unsigned int)time(NULL));	// 乱数初期化
		auto type = rand() % 3 + 1;
		switch (type) {
		case 1:
			soul->SetParameter(1, 7);	// 赤の魂
			break;
		case 2:
			soul->SetParameter(2, 7);	// 青の魂
			break;
		default:
			break;
		}
		// 自機は魂の所持上限に到達しているか？
		if (player->IsSoulMax()) {
			_soul->OwnerNull();	// 所有者はいない
			soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
			_game.GetObjectServer()->Add(std::move(soul));	// オブジェクトサーバーに登録する
			return;
		} else player->SoulCatch(std::move(soul));	// そうではない場合は魂の所有権をプレイヤーに譲渡
	}

	int CrowDoll::IsAnger() {
		// 怒り状態に突入しているか？
		if (CROW_ANGER < _life) return IS_NORMAL;	// 怒り状態になっていない
		return IS_ANGER;	// 怒っている
	}

	AABB CrowDoll::NowCollision(std::string key) {

		return _mainCollision;
	}
}
