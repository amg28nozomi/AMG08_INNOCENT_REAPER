#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "TrackingEffect.h"
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
	constexpr auto DEFAULT_Y = 400; // 870 - 300;

	constexpr auto IS_ANGER = 1;
	constexpr auto IS_NORMAL = 0;

	constexpr auto RASH_MAX = 250;

	constexpr auto FLOAT_MAX = 540;
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
			{enemy::crowdoll::CROW_RUSH, {_position, 210, -30, CROW_HEIGHT / 2 , CROW_HEIGHT / 2, true}},	// 連撃攻撃の当たり判定
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};

		// { 画像検索用キー, {総フレーム数, SEの再生時間} }
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {enemy::crowdoll::motion::IDOL * 2, 0}},
			{enemy::crowdoll::CROW_RUSH , {enemy::crowdoll::motion::RUSH  * 2, 20}},
			{enemy::crowdoll::CROW_BLINK , {enemy::crowdoll::motion::BLINK * 2, 20}},
			{enemy::crowdoll::CROW_GROWARM , {enemy::crowdoll::motion::GROWARM * 3, 20}},
			{enemy::crowdoll::CROW_ROAR , {enemy::crowdoll::motion::ROAR * 4, 50}},
			{enemy::crowdoll::CROW_DEBUF, {enemy::crowdoll::motion::DEBUF * 3, 50}},
			{enemy::crowdoll::CROW_DOWN , {enemy::crowdoll::motion::DOWN * 3, 50}},
			{enemy::crowdoll::CROW_WINCE, {enemy::crowdoll::motion::WINCE * 3, 50}},
		};
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_actionCount = 0;
		_muteki = 0;
		_life = 10;
		_arm = false;
		_setup = false;
		_changeGraph = false;
		_wait = false;
		_direction = true;
		_changeState = false;
		_isAnimation = false;
		_isWarp = false;
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
		if (_isWarp == true && _atkInterval == 0) Warp();
		SetState();
		Floating();
		Attack();	// ダメージ処理
		Move();
	}

	void  CrowDoll::Draw() {

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
			ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
			_isAnimation = true;
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
		
		if (_cState != CrowState::RUSH) return;	// 攻撃ボックスがない場合は処理を行わない
		if (_atkInterval != 0) return;	// 待ち時間中は判定を行わない
		auto damageBox = _collisions.find(_divKey.first);	// ボックスはあるか？
		if (damageBox == _collisions.end()) return;
		if(damageBox->second.HitCheck(playerBox) == true) player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// 座標方向に飛ばす
	}

	bool CrowDoll::IsGravity() {
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
		case CrowState::RUSH:
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				_gravity = 0;
				_stand = false;
				return true;
			}
			_gravity += FRAME_G;	// 加速度を加算
			if (MAX_G < _gravity) _gravity = MAX_G;
			if (IsStandChip()) {
				if (0 < _gravity) _stand = true;
				if (_cState == CrowState::BLINK) {
					_gravity = 0;
					// WarpOn();	// ワープ移動
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
		if (_cState == CrowState::IDOL || _cState == CrowState::ROAR) {
			if (_position.GetY() <= DEFAULT_Y) return false;
			_gravity -= 0.25;
			if (_position.GetY() + _gravity < DEFAULT_Y) {
				_position.GetPY() = DEFAULT_Y;
				_gravity = 0;
			}
			return true;
		}
		return false;
	}

	void CrowDoll::Warp() {
		_isWarp = false;
		_isAnimation = true;	// アニメーション再開
		AddWarpEffect(_warpPos, true);	// 追従処理有り
		_position = _warpPos;

		switch (_cState) {
		case CrowState::RUSH:
			PlaySe(enemy::crowdoll::SE_RUSH);
			ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_RUSH);
			AddRushEffect();
			return;
		case CrowDoll::CrowState::DEBUF:
			return;
		case CrowDoll::CrowState::ROAR:
			return;
		case CrowDoll::CrowState::BLINK:
			ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_BLINK);	// 状態切り替え
			AddBlinkEffect();
			return;
		case inr::CrowDoll::CrowState::IDOL:
			return;
		default:
			return;
		}
	}

	void CrowDoll::Rash() {
		_isAnimation = true;
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
		if (_actionCount == 1) _atkInterval = 30;
		else _atkInterval = 20;
		_isAnimation = false;
		AttackBox(false);
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
		return false;
	}

	bool CrowDoll::SetState() {
		if (_isWarp == true) return false;	// 転移処理がある場合はスキップ
		// 状態に応じた処理を行う
		switch (_cState) {
		case CrowState::IDOL:	// 空中待機の場合
			// インターバル明けに次のアクションを実行する
			if (_atkInterval == 0) {
				auto number = rand() % 3; // %3
				switch (number) {
				case 0:
					ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					GetTarget();	// 自機の現在座標を取得する
					WarpOn();	// 自機の前に跳ぶ
					_actionCount = 4;
					_atkInterval = 30;
					break;
				case 1:
					ModeChange(CrowState::GROWARM, enemy::crowdoll::CROW_GROWARM);	// 状態切り替え
					GetTarget();
					PlaySe(enemy::crowdoll::SE_GROWARM);
					break;
				case 2:
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_actionCount = IsAnger();	// 切れている場合は処理を追加で実行する
					WarpOn();	// 自機の前に跳ぶ
					// _atkInterval = 60;
					break;
				}
			}
			break;
		case CrowState::RUSH:
			if (_atkInterval == 0) {	// 待ち時間がない場合
				if (0 < _actionCount) {
					AttackBox(true);
					Rash();	// ラッシュアクション実行
					break;
				}
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_atkInterval = 60;
					_isAnimation = true;
				}
				// 次の状態に遷移する
				break;
			}
			break;
		case CrowState::BLINK:
			// 立っている場合
			if (_stand == true) {
				if (_wait != true) {
					_wait = true;
					_atkInterval = 60;
					break;
				}
				else if (AnimationCountMax() == true) {
					// 待ち時間は終了しているか？
					if (_atkInterval == 0) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
						_atkInterval = 60;
						_wait = false;
						_isAnimation = true;
					} else if (_isAnimation == true) _isAnimation = false;
				}
				break;
			}
			break;
		case CrowState::WINCE:
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
			break;
		case CrowState::GROWARM:
			// 腕を挿した瞬間にエフェクトを発生させる
			if (IsAnimationMax() == true && _arm == false) {	// モーションが最大かつ、腕が未生成の場合のみ腕を生成する
				auto effarm = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ARM, Vector2(_target.GetX(), 655), 24 * 2);	// エフェクトを作成(950)
				effarm->SetDamageEffect(50, 50, 0, 300,  10);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(effarm), effect::type::FORMER);
				_arm = true;
				break;
			}
			else if (AnimationCountMax() == true) {	// カウントがマックになった場合、状態を切り替える
				_arm = false;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
			break;

		case CrowState::ROAR:	// 咆哮処理
			if (_divKey.first != enemy::crowdoll::CROW_ROAR) {
				// y座標が一定地点まで到達したら咆哮を開始する
				if (_position.IntY() == DEFAULT_Y) {
					_actionCount = 3;	// 四回繰り返す
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_ROAR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
					ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_ROAR);	// 状態切り替え
					auto roar_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ROAR, _position, 40);// _game.GetMapChips()->GetWorldPosition(), 30);
					roar_eff->SetLoop(3);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(roar_eff), effect::type::FORMER);
					break;
				}
			} else if (AnimationCountMax() == true) {
				if (_actionCount == 0) {
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
					break;
				}
				--_actionCount;
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
					// 各種値を消去
					_atkInterval = 0;
					_actionCount = 0;
					_isAnimation = true;
					_wait = false;
					_moveVector = {};

					// 魂を奪われる
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// 怯み状態にする
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
					AddSoul();	// 魂を生み出す
					--_life;
					if(_life == 0) ModeChange(CrowState::SLEEP, enemy::crowdoll::CROW_DOWN);	// 死亡判定
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
		if (_isAnimation == true )ObjectBase::AnimationCount();	// カウントを増やす
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
		_game.GetObjectServer()->Add(soul);	// オブジェクトサーバーに登録する
		soul->SetSpwan(_position);	// 自身の中心座標に実体化させる
		if (player->IsSoulMax() == true) {
			soul->OwnerNull();	// 所有者はいない
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

	bool CrowDoll::AddWarpEffect(Vector2 spwan, bool target) {
		if (target == true) {
			auto warp_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK, spwan, 24 * 2);
			warp_eff->Set(this);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(warp_eff), effect::type::FORMER);
			return true;
		} else {
			auto warp_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::BLINK, spwan, 30);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(warp_eff), effect::type::FORMER);
			return true;
		}
	}

	bool CrowDoll::AddRushEffect() {
		bool eff_dir = _direction == enemy::MOVE_LEFT;

		auto rush_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::RUSH, _position, 47 * 3, _direction);
		rush_eff->Set(this, -30, -30);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(rush_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::AddBlinkEffect() {
		auto blink_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK_ATTACK, _position, effect::crow::BLINL_ATTACK_MAX * 3);
		blink_eff->Set(this, 0, -150);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(blink_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::IsPlayerPosition() {
		auto player_p = _target;
		_game.GetMapChips()->Clamp(player_p);
		// 自機は左右どちら側に居るのか
		if (player_p.IntX() < HALF_WINDOW_W) return false;	// 左側にいる
		else if (HALF_WINDOW_W < player_p.IntX()) return true;	// 右側にいる
		// 中心座標にいる場合はランダムで判定
		switch (rand() % 2) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
		}
	}

	AABB CrowDoll::NowCollision(std::string key) {

		return _mainCollision;
	}

	void CrowDoll::WarpOn() {
		if (_isWarp == true) return;
		_isWarp = true;	// 転移
		
		AddWarpEffect(_position);	// 転移エフェクトの生成
		// SEを鳴らす

		double px = 0;
		int sound = 0;

		bool isPlayer = IsPlayerPosition();

		if (isPlayer == true) {	// 自機は左に居る
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_LEFT;
				px = (_game.GetMapChips()->GetWorldPosition().GetX() + 700 + _mainCollision.GetWidthMax());
				_warpPos = { px, 820 };	// 転移座標に代入する
				_actionEnd.GetPX() = px - RASH_MAX;
				break;
			default:
				break;
			}
		}
		else {
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_RIGHT;
				px = (_game.GetMapChips()->GetWorldPosition().GetX() - 700 - _mainCollision.GetWidthMin());
				_warpPos = { px, 820 };
				_actionEnd.GetPX() = px + RASH_MAX;
				break;
			default:
				break;
			}
		}
		switch (_cState) {
		case inr::CrowDoll::CrowState::DEBUF:
			//中心座標にワープする
			_warpPos = { static_cast<double>(_game.GetMapChips()->GetMapSizeWidth()) - HALF_WINDOW_W, HALF_WINDOW_H };
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
			_warpPos = { _target.GetX(), DEFAULT_Y };	// 座標変更
			_moveVector = { 0, 0 };	// 移動量は消す
			break;
		case inr::CrowDoll::CrowState::IDOL:
			_warpPos = _game.GetMapChips()->GetWorldPosition();
			_moveVector = { 0, 0 };
			break;
		default:
			break;
		}

		_atkInterval = 15;	// 10フレーム後にアクションを実行する
	}

	bool CrowDoll::AttackBox(bool flag) {
		auto rush = _collisions.find(enemy::crowdoll::CROW_RUSH);
		if (rush->second.GetCollisionFlg() == flag) return false;
		rush->second.GetCollisionFlgB() = flag;
#ifdef _DEBUG
		rush->second.GetbDrawFlg() = flag;
#endif
		return true;
	}
}
