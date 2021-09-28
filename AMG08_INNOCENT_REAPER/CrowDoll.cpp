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
	constexpr auto CROW_HEIGHT = 260;	// 縦の当たり判定

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
		_cState = CrowState::SLEEP;
		Init();
	}

	void CrowDoll::Init() {
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// 当たり判定
		_collisions = {
			{enemy::crowdoll::CROW_RUSH, {_position, 160, -30, CROW_HEIGHT / 2 , 10, true}},	// 連撃攻撃の当たり判定
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {60, 0}},
			{enemy::crowdoll::CROW_RUSH , {70, 20}},
			{enemy::crowdoll::CROW_BLINK , {13 * 3, 20}},
			{enemy::crowdoll::CROW_GROWARM , {30, 20}},
			{enemy::crowdoll::CROW_ROAR , {11 * 3, 50}},
			{enemy::crowdoll::CROW_DEBUF, {22 * 3, 50}},
			{enemy::crowdoll::CROW_DOWN , {26 * 3, 50}},
			{enemy::crowdoll::CROW_WINCE, {7 * 3, 50}},
		};
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_actionCount = 0;
		_muteki = 0;
		_life = 10;
		_arm = false;
		_setup = false;
		_changeGraph = false;
		_direction = true;
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
		Attack();	// ダメージ処理
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
			_mainCollision.GetCollisionFlgB() = true;	// 当たり判定をオンにする
			auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
			ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_ROAR);	// 状態切り替え
			auto roar_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ROAR, _position, 30);// _game.GetMapChips()->GetWorldPosition(), 30);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(roar_eff), effect::type::FORMER);
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

		for (auto&& it : _collisions) it.second.Update(_position, _direction);
		_moveVector = { 0, 0 };
	}

	void CrowDoll::Attack() {
		// 攻撃処理
		auto player = _game.GetObjectServer()->GetPlayer();	// 自機
		auto playerBox = player->GetMainCollision();	// プレイヤーの当たり判定
		
		// まずは敵の当たり判定と接触判定を行う
		if (_mainCollision.HitCheck(playerBox) == true) {
			// 当たったか？
			player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// 座標方向に飛ばす
			return;	// 攻撃判定が入った場合は処理を終了する
		}
		
		auto damageBox = _collisions.find(_divKey.first);	// ボックスはあるか？
		if (damageBox == _collisions.end()) return;
		if(damageBox->second.HitCheck(playerBox) == true) player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// 座標方向に飛ばす
	}

	bool CrowDoll::IsGravity() {
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::RUSH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
			_gravity += FRAME_G;	// 加速度を加算
			if (MAX_G < _gravity) _gravity = MAX_G;
			if (IsStandChip()) {
				if (0 < _gravity) _stand = true;
				if (_cState == CrowState::BLINK) {
					_atkInterval = 30;
					_gravity = 0;
					Warp();	// ワープ移動
					return true;
				}	// ワープ処理を呼び出す
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
		if (_position.GetY() <= DEFAULT_Y) return false;
		_gravity -= 0.25;
		if (_position.GetY() + _gravity < DEFAULT_Y) { 
			_position.GetPY() = DEFAULT_Y;
			_gravity = 0;
		}
		return true;
	}

	void CrowDoll::Warp() {
		auto p = _position.GetX() - _target.GetX();	// 自機はどちら側にいるか？
		double px = 0;
		int sound = 0;
		if (p < 0) {	// 自機は左に居る
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_RIGHT;
				px = (_target.GetX() - (_mainCollision.GetWidthMin()) * 5);
				_position = { px, 870 };
				_actionEnd.GetPX() = px - RASH_MAX;
				sound = se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH);
				PlaySoundMem(sound, se::SoundServer::GetPlayType(enemy::crowdoll::SE_RUSH));
				break;
			default:
				break;
			}
		}// 自機は右側にいる
		else if (0 < p) { 
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_LEFT;
				px = (_target.GetX() + (_mainCollision.GetWidthMax()) * 5);
				_position = { px, 870 };
				_actionEnd.GetPX() = px + RASH_MAX;
				sound = se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH);
				PlaySoundMem(sound, se::SoundServer::GetPlayType(enemy::crowdoll::SE_RUSH));
				break;
			default:
				break;
			}
		}// 自機は左側にいる
		switch (_cState) {
		case inr::CrowDoll::CrowState::DEBUF:
			//中心座標にワープする
			_position = { static_cast<double>(_game.GetMapChips()->GetMapSizeWidth()) - HALF_WINDOW_W, HALF_WINDOW_H};
			_moveVector = { 0, 0 };
			break;
		case inr::CrowDoll::CrowState::ROAR:
			/*if (AnimationCountMax() == true) {
				_atkInterval = 30;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
				Warp();
			}*/
			break;
		case inr::CrowDoll::CrowState::BLINK:
			// 自機の頭上にワープする
			GetTarget();
			_position = { _target.GetX(), _target.GetY() - 600 };	// 座標変更
			_moveVector = { 0, 0 };	// 移動量は消す
			break;
		case inr::CrowDoll::CrowState::IDOL:
			_position = _game.GetMapChips()->GetWorldPosition();
			_moveVector = {};
		default:
			break;
		}

		_atkInterval = 10;	// 10フレーム後にアクションを実行する
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
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
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
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
				return;
			}
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RASH_MAX;
			break;
		}
		--_actionCount;
		_atkInterval = 20;
	}

	void CrowDoll::Blink() {
		_gravity += 0.5;	// 重力加速
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
				auto number = rand() % 3;
				switch (number) {
				case 0:
					ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_RUSH);	// 状態切り替え
					GetTarget();	// 自機の現在座標を取得する
					Warp();	// 自機の前に跳ぶ
					_actionCount = 4;
					break;
				case 1:
					ModeChange(CrowState::GROWARM, enemy::crowdoll::CROW_GROWARM);	// 状態切り替え
					GetTarget();
					break;
				case 2:
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_actionCount = IsAnger();	// 切れている場合は処理を追加で実行する
					Warp();	// 自機の前に跳ぶ
					_atkInterval = 60;
					break;
				}
			}
			break;
		case CrowState::RUSH:
			if (_atkInterval == 0) {
				if (0 < _actionCount) {
					Rash();	// ラッシュアクション実行
					break;
				}
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_actionCount = IsAnger();	// 切れている場合は処理を追加で実行する
					_atkInterval = 60;
				}
				// 次の状態に遷移する
				break;
			}
		case CrowState::BLINK:
			if (_atkInterval == 0) {
				if (0 != _actionCount) {
					Blink();	// ラッシュアクション実行
					break;
				}
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
		case CrowState::WINCE:
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
			break;
		case CrowState::GROWARM:
			// 腕を挿した瞬間にエフェクトを発生させる
			if (AnimationNumber() == 4 && _arm == false) {
				auto effarm = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ARM, Vector2(_target.GetX(), 650), 24 * 2);	// エフェクトを作成(950)
				effarm->SetDamageEffect(300, 300, 9);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(effarm), effect::type::FORMER);
				_arm = true;
				break;
			}
			else if (IsAnimationMax() == true) {
				_arm = false;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
			break;

		case CrowState::ROAR:
			if (AnimationCountMax() == true) {
				_atkInterval = 30;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
				Warp();
			}
			break;
		}

		return true;
	}

	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_cState == CrowState::DEATH) return;	// 死んでいる場合は魂を奪えない
		if (_muteki != 0) return;	// 無敵時間がある場合も奪えない
		if (IsVital() != true) return;	// 隙がない場合も魂を奪えない
		if (ckey == PKEY_ROB) {	// 魂を奪いにきているか？
			auto vitalPart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// 魂を奪われる
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// 怯み状態にする
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
					AddSoul();	// 魂を生み出す
					--_life;
					if(_life == 0) ModeChange(CrowState::SLEEP, enemy::crowdoll::CROW_DOWN);	// 怯み状態にする
					// ここで死亡処理を行うか判定を行う
					_muteki = 60;	// 一定時間の間、無敵状態にする
					return;
			}
		}
	}

	bool CrowDoll::IsVital() {
		switch (_cState) {
		case CrowState::DEBUF:
		case CrowState::ROAR:
		case CrowState::WINCE:
		case CrowState::SLEEP:
			return false;
		case CrowState::RUSH:
		case CrowState::BLINK:
		case CrowState::IDOL:
			return true;
		default:
			return false;
		}
	}

	bool CrowDoll::IsActive() {
		if (_setup != true) return false;
		if (_muteki != 0) --_muteki;	// 無敵時間がある場合は減らすある
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
		// 自機は魂の所持上限に到達しているか？
		if (player->IsSoulMax() == true) {
			soul->OwnerNull();	// 所有者はいない
			soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
			_game.GetObjectServer()->Add(std::move(soul));	// オブジェクトサーバーに登録する
			return;
		} else player->SoulCatch(std::move(soul));	// そうではない場合は魂の所有権をプレイヤーに譲渡
	}

	bool CrowDoll::IsPlayerPos(double px) {
		auto fix = _position.GetX() - px;
		if (fix < 0) return false;
		if (0 < fix) return true;
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
