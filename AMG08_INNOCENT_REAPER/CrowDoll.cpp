#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "TrackingEffect.h"
#include "LoopEffect.h"
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

	constexpr auto LIFE_MAX = 10; // 10

	constexpr auto DEBUFF_COUNT_MIN = 0;
	constexpr auto DEBUFF_COUNT_MAX = 5;

	constexpr auto DEBUFF_START = 6;
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;	
		_aState = ActionState::IDOL;
		_eType = EnemyType::CROW_DOLL;
		_cState = CrowState::SLEEP;
		Init();
	}

	// 初期化
	void CrowDoll::Init() {
		// 各種初期化
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// 当たり判定
		_collisions = {
			// 連撃切りの当たり判定
			{enemy::crowdoll::CROW_RUSH, {_position, 210, -30, CROW_HEIGHT / 2 , CROW_HEIGHT / 2, true}},
			// 抜け殻時の当たり判定
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};

		// { 画像検索用キー, {総フレーム数, SEの再生時間} }
		_motionKey = {
			// 待機状態
			{enemy::crowdoll::CROW_IDOL, {enemy::crowdoll::motion::IDOL * 2, 0}},
			// 連続切り
			{enemy::crowdoll::CROW_RUSH , {enemy::crowdoll::motion::RUSH  * 2, 20}},
			// 落下攻撃
			{enemy::crowdoll::CROW_BLINK , {enemy::crowdoll::motion::BLINK * 2, 20}},
			// 伸びる腕
			{enemy::crowdoll::CROW_GROWARM , {enemy::crowdoll::motion::GROWARM * 3, 20}},
			// 咆哮
			{enemy::crowdoll::CROW_ROAR , {enemy::crowdoll::motion::ROAR * 4, 50}},
			// デバフ
			{enemy::crowdoll::CROW_DEBUFF, {enemy::crowdoll::motion::DEBUF * 3, 50}},
			// ダウン
			{enemy::crowdoll::CROW_DOWN , {enemy::crowdoll::motion::DOWN * 3, 50}},
			// 怯み
			{enemy::crowdoll::CROW_WINCE, {enemy::crowdoll::motion::WINCE * 5, 50}},
		};
		// 初期化
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_actionCount = 0;
		_muteki = 0;
		_life = LIFE_MAX;
		_arm = false;
		_setup = false;
		_changeGraph = false;
		_wait = false;
		_direction = true;
		_changeState = false;
		_isAnimation = false;
		_isWarp = false;
		_debuf = false;
		_debuffChage = DEBUFF_COUNT_MIN;
	}

	// オブジェクト情報の登録
	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// 登録
		_position = _oValue.Positions().at(0);	// 座標を更新
		Init();	// 初期化
	}

	// 更新
	void CrowDoll::Process() {
		IsGravity();	// 重力処理
		IsBattle();	// ボス戦闘フラグ
		WakeUp();	// 起き上がり
		if (IsActive() != true) return;	// 活動状態でない場合は処理を行わない
		if (_isWarp == true && _atkInterval == 0) Warp();	// ワープ処理
		SetState();	// 状態の設定
		Floating();	// 浮遊処理
		Attack();	// ダメージ処理
		Move();	// 移動処理
	}

	// 描画
	void  CrowDoll::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// 座標をクランプする
		// 描画座標の算出
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用

		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);	// 当たり判定の描画
		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		// 各種判定の描画(描画処理がオンの場合のみ)
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlag() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
#endif
	}

	// 活動開始
	void CrowDoll::WakeUp() {
		// ボス戦が始まっていない場合は終了
		if (_game.GetModeServer()->GetModeMain()->BossFlag() != true) return;
		if (_setup == true) return;	// 準備が完了している場合も終了
		--_aCount;	// カウンタ減算(逆再生)
		if (_aCount == 0) {	// カウンタが0になった場合
			_setup = true;	// セットアップ完了
			_mainCollision.SetCollisionFlag() = true;	// 当たり判定をオンにする
			ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
			_isAnimation = true;	// アニメーション再生を開始
			return;
		}
	}

	// 自機の座標を取得
	void CrowDoll::GetTarget() {
		_target = _game.GetObjectServer()->GetPlayer()->GetPosition();	// 目標の現在地点を取得する
	}

	// 向きの更新
	void CrowDoll::ChangeDirection() {
		// 移動方向に応じて向きを変更する
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}

	// 状態遷移
	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;	// 画像キー切り替え
		_cState = nextState;	// 状態遷移
		_changeGraph = true;	// 画像切り替えフラグオン
	}

	// 移動処理
	void CrowDoll::Move() {
		ChangeDirection();	// 向きの調整
		_moveVector.GetPY() = _gravity;	// y座標に重力値加算」
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);	// 押し出し処理

		_position = _position + _moveVector;	// 座標更新
		_mainCollision.Update(_position, _direction);	// 当たり判定の更新

		for (auto&& it : _collisions) it.second.Update(_position, _direction);	// 当たり判定の更新
		_moveVector = { 0, 0 };	// 移動量を初期化する
	}

	// 攻撃処理
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

	// 重力処理を行うか？
	bool CrowDoll::IsGravity() {
		// 現在の状態に応じて重力処理を行うかの判定を行う
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
		case CrowState::RUSH:
			// 描画画像が待機状態の場合
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				_gravity = 0;	// 重力値初期化
				_stand = false;	// 立っていない
				return true;
			}
			_gravity += FRAME_G;	// 加速度を加算
			if (MAX_G < _gravity) _gravity = MAX_G;	// 上限値を超過している場合は修正
			if (IsStandChip()) {	// 地面に立っているか？
				if (0 < _gravity) _stand = true;
				if (_cState == CrowState::BLINK) {
					_gravity = 0;
					// WarpOn();	// ワープ移動
					return true;
				}
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

	// 浮遊処理
	bool CrowDoll::Floating() {
		// 怯み状態の場合
		if (_cState == CrowState::WINCE) {
			// 現在のY座標が浮遊上限以下の場合
			if (_position.GetY() + _gravity <= DEFAULT_Y) {
				_position.GetPY() = DEFAULT_Y;	// 座標を修正
				_gravity = 0;	// 重力値初期化
				return true;
			}
		}
		// 待機状態または咆哮状態の場合
		if (_cState == CrowState::IDOL || _cState == CrowState::ROAR) {
			if (_position.GetY() <= DEFAULT_Y) return false;
			_gravity -= 0.25;	// 重力値減算
			if (_position.GetY() + _gravity < DEFAULT_Y) {
				_position.GetPY() = DEFAULT_Y;
				_gravity = 0;
			}
			return true;
		}
		return false;
	}

	// ワープ処理
	void CrowDoll::Warp() {
		_isWarp = false;	// ワープフラグをオフにする
		_isAnimation = true;	// アニメーション再開
		AddWarpEffect(_warpPos, true);	// 追従処理有り
		_position = _warpPos;	// 座標を切り替える
		
		// 状態に応じた処理を実行
		switch (_cState) {
		case CrowState::RUSH:
			PlaySe(enemy::crowdoll::SE_RUSH);
			ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_RUSH);	// 状態切り替え
			AddRushEffect();	// 連撃エフェクト発生
			return;
		case CrowDoll::CrowState::DEBUF:
			ModeChange(CrowState::DEBUF, enemy::crowdoll::CROW_DEBUFF);	// 状態切り替え
			return;
		case CrowDoll::CrowState::ROAR:
			return;
		case CrowDoll::CrowState::BLINK:
			ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_BLINK);	// 状態切り替え
			AddBlinkEffect();	// 落下エフェクト発生
			return;
		case inr::CrowDoll::CrowState::IDOL:
			return;
		default:
			return;
		}
	}

	// 連続攻撃
	void CrowDoll::Rash() {
		_isAnimation = true;	// アニメーション再生開始

		double mx = 0;
		double nextpos = 0;
		switch (_direction) {
		case enemy::MOVE_LEFT:	// 左向きの場合
			mx = -RASH_MAX / 15;	// 移動量算出
			_moveVector.GetPX() = mx;	// 移動ベクトルに代入
			nextpos = _position.GetX() + mx;	// 移動後の座標を算出
			if (IsAttackEnd() == true) {	// マップチップに衝突したか？
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));	// SEを止める
				return;
			}
			// 移動後のX座標が目標座標に満たない場合は処理を終了
			if (_actionEnd.GetX() < nextpos) return;
			_actionEnd.GetPX() = nextpos - RASH_MAX;	// 目標座標の更新
			break;
		case enemy::MOVE_RIGHT:	// 右向きの場合
			mx = RASH_MAX / 15;	// 移動量算出
			_moveVector.GetPX() = mx;	// 移動ベクトルに代入
			nextpos = _position.GetX() + mx;	// 移動後の座標を算出
			if (IsAttackEnd() == true) {	// マップチップに衝突したか？
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
				return;
			}
			// 移動後のX座標が目標座標に満たない場合は処理を終了
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RASH_MAX;	// 目標座標の更新
			break;
		}
		
		--_actionCount;	// アクションカウンタを減らす
		// アクションカウンタに応じて猶予時間を切り替え
		if (_actionCount == 1) _atkInterval = 30;
		else _atkInterval = 20;
		_isAnimation = false;	// アニメーション中断
		AttackBox(false);	// 攻撃判定を切る
	}

	// 攻撃を終了するか？（マップチップと衝突しているか？）
	bool CrowDoll::IsAttackEnd() {
		// マップチップに衝突しているか？
		if (_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction) == mapchip::NORMAL) return true;
		return false;
	}

	// 各種状態の管理
	bool CrowDoll::SetState() {
		if (_isWarp == true) return false;	// 転移処理がある場合はスキップ
		// 状態に応じた処理を行う
		switch (_cState) {
			// 待機状態の場合
		case CrowState::IDOL:
			// インターバルはあるか？
			if (_atkInterval == 0) {
				// 怒り状態かつ、デバフの溜めカウンタが上限に到達している場合
				if (IsAnger() == IS_ANGER && _debuffChage == DEBUFF_COUNT_MAX) {
					// 確率でデバフ状態に遷移（確率50%）
					auto number = rand() % 11 + 1;
					if (_debuffChage < number) {
						_debuffChage = 0;	// 溜めカウンタを初期化
						ModeChange(CrowState::DEBUF, enemy::crowdoll::CROW_IDOL);
						_muteki = 120;	// 無敵状態に設定
						return true;
					}
				}
				auto number = rand() % 3; // ランダムで状態遷移
				switch (number) {
				case 0:
					ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					GetTarget();	// 自機の現在座標を取得する
					WarpOn();	// 自機の前に跳ぶ
					_actionCount = 4;	// 四段階攻撃
					_atkInterval = 30;	// 攻撃猶予時間
					break;
				case 1:
					ModeChange(CrowState::GROWARM, enemy::crowdoll::CROW_GROWARM);	// 状態切り替え
					GetTarget();	// 自機の座標取得
					PlaySe(enemy::crowdoll::SE_GROWARM);
					break;
				case 2:
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_actionCount = IsAnger();	// 切れている場合は処理を追加で実行する
					WarpOn();	// 自機の前に跳ぶ
					break;
				}
			}
			break;
			// 連続切り状態の場合
		case CrowState::RUSH:
			if (_atkInterval == 0) {	// 待ち時間がない場合
				// アクションカウンタがある場合
				if (0 < _actionCount) {
					AttackBox(true);	// 攻撃判定をオン
					Rash();	// ラッシュアクション実行
					break;
				}
				// アクションカウンタが0の場合
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;	// 初期化
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
					_atkInterval = 60;
					_isAnimation = true;
					// 怒り状態の場合、デバフの溜めカウンタを増やす（上限有り）
					if(IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
				}
				break;
			}
			break;
			// 落下攻撃
		case CrowState::BLINK:
			// 立っている場合
			if (_stand == true) {
				if (_wait != true) {
					AddSmokeEffect();	// エフェクト生成
					_wait = true;	// 処理を止める
					_atkInterval = 60;	// 猶予時間設定
					break;
				}
				// アニメーションカウンタが上限に到達した場合
				else if (AnimationCountMax() == true) {
					// 待ち時間は終了しているか？
					if (_atkInterval == 0) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
						_atkInterval = 60;
						_wait = false;
						_isAnimation = true;
						if (IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
					}
					// 終了していない場合、アニメーションの再生を行わない
					else if (_isAnimation == true) _isAnimation = false;
				}
				break;
			}
			break;
				// 怯み状態の場合
		case CrowState::WINCE:
			// アニメーションカウンタが上限に到達した場合
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				break;
			}
			break;
			// 伸びる腕状態の場合
		case CrowState::GROWARM:
			// モーションが最大かつ、腕を未生成の場合
			if (IsAnimationMax() == true && _arm == false) {
				// エフェクトの生成および登録
				auto effarm = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ARM, Vector2(_target.GetX(), 655), 24 * 2);
				effarm->SetDamageEffect(50, 50, 0, 300, 10);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(effarm), effect::type::FORMER);
				_arm = true;	// 腕を生成した
				break;
			}
			else if (AnimationCountMax() == true) {	// カウントがマックになった場合、状態を切り替える
				_arm = false;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// 状態切り替え
				_atkInterval = 60;
				if (IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
				break;
			}
			break;

		case CrowState::ROAR:	// 咆哮状態の場合
			// 画像キーが咆哮ではない場合
			if (_divKey.first != enemy::crowdoll::CROW_ROAR) {
				// y座標が一定地点まで到達したら咆哮を開始する
				if (_position.IntY() == DEFAULT_Y) {
					_actionCount = 3;	// 四回繰り返す
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_ROAR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
					ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_ROAR);	// 状態切り替え
					// 咆哮エフェクトの生成および登録
					auto roar_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ROAR, _position, 40);
					roar_eff->SetLoop(3);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(roar_eff), effect::type::FORMER);
					_game.GetObjectServer()->GetPlayer()->KnockBack(IsPlayerPos(_moveVector.GetX()));
					DollsEnd();	// 魂を奪い取る
					break;
				}
			}
			// アニメーションカウンタが上限に到達した場合
			else if (AnimationCountMax() == true) {
				// インターバルがない場合
				if (_actionCount == 0) {
					// 待機状態に遷移する
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
					break;
				}
				--_actionCount;
			}
			break;
			// デバフ状態
		case CrowState::DEBUF:
			// 画像キーが待機状態の場合
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				if (_atkInterval == 0 && AnimationCountMax() == true && _isWarp != true) {
					WarpOn();	// ワープ
					PlaySe(enemy::crowdoll::SE_DEBUF);	// SEを鳴らす
					_atkInterval = 20;
				}
				break;
			}
			// 画像キーがでデバフの場合
			if (_divKey.first == enemy::crowdoll::CROW_DEBUFF) {
				// アニメーション番号が一致してかつ、デバフフラグが偽の場合
				if (AnimationNumber() == DEBUFF_START && _debuf == false) {
					_debuf = true;
					_isAnimation = false;
					_atkInterval = 60;
					return true;
					// デバフフラグが真の場合
				} if (_debuf == true) {
					if (_atkInterval == 0 && _isAnimation != true) {
						_isAnimation = true;
						AddDebufEffect();	// デバフ生成
						return true;
					// アニメーションカウンタが上限に到達した場合
					} else if (AnimationCountMax() == true) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
						_debuf = false;	// デバフフラグを偽にする
						_atkInterval = 60;
					}
				}
				break;
			}

			return true;
		}
	}

	// 死んでいるか？
	bool CrowDoll::IsDead() {
		if (_cState != CrowState::DEATH) return false;
		return true;
	}

	// 自機アクションとの判定
	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_cState == CrowState::DEATH) return;	// 死んでいる場合は終了
		if (_muteki != 0) return;	// 無敵時間がある場合は中断
		if (IsVital() != true) return;	// 隙がない場合も中断
		// 自機は奪うアクション中か？
		if (ckey == PKEY_ROB) {
			// 落下攻撃中(落下モーション中)は通常の当たり判定と判定を行う
			if (_cState == CrowState::BLINK && _divKey.first == enemy::crowdoll::CROW_BLINK) {
				auto vital = BlinkVitalPart(_mainCollision, CROW_VITAL);	// 急所の算出
				if (vital.first.HitCheck(acollision) == true || vital.second.HitCheck(acollision) == true) {
					_direction = !direction;	// 向きを反転させる
				} else return;
			} else if (_cState != CrowState::BLINK) {
				auto vitalPart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalPart.HitCheck(acollision) == true) {
					/* 処理 */
				} else return;
			}
			// 各種変数を初期化
			_atkInterval = 0;
			_actionCount = 0;
			_isAnimation = true;
			_wait = false;
			_moveVector = {};

			// 死亡処理が真の場合は処理を終了
			if (DeathOn() == true) return;

			// 怯み状態にする
			ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);
			auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// 鳴き声を鳴らす
			AddSoul();	// 魂を生み出す
			--_life;	// 耐久値を減らす
			// 怒り状態に突入した場合は処理を終了
			if(AngerOn() == true) return;
			_muteki = 60;	// 無敵時間の設定
			return;
		}
	}

	// 魂を奪える状態かの判定
	bool CrowDoll::IsVital() {
		// 現在の状態は魂を奪えるか？
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

	// 活動状態にあるか？
	bool CrowDoll::IsActive() {
		// 準備が完了していない場合は処理を終了
		if (_setup != true) return false;
		if (_muteki != 0) --_muteki;	// 無敵時間がある場合は減らすある
		// 死亡状態または非活動状態の場合
		if (_cState == CrowState::DEATH || _cState == CrowState::SLEEP) {
			if (AnimationCountMax() != true) ObjectBase::AnimationCount();
			return false;
		}
		if (0 < _atkInterval) --_atkInterval;	// 待ち時間を減らす
		if (_isAnimation == true )	ObjectBase::AnimationCount();	// カウントを増やす
		return true;
	}

	bool CrowDoll::IsBattle() {
		if (_cState == CrowState::DEATH) return false;	// 死んでいるか？
		if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return false;	// ボス戦闘フラグがオンになっているか？
		// 自機のX座標は目標地点まで到達しているか？
		if (_game.GetObjectServer()->GetPlayer()->GetPosition().GetX() < _game.GetMapChips()->GetMapSizeWidth() - HALF_WINDOW_W) return false;
		_game.GetModeServer()->GetModeMain()->BossBattle();	// 到達している場合はボス戦闘処理呼び出し
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
			auto warp_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::BLINK, spwan, 30);
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
		auto blink_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK_ATTACK, _position, effect::crow::BLINL_ATTACK_MAX * 4);
		blink_eff->Set(this, 0, -150);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(blink_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::AddSmokeEffect() {
		Vector2 smoke_pos = { _position.GetX(), (_position.GetY() + ((_mainCollision.GetHeightMax() / 2) - 75)) };
		auto smoke_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy::HITDROP, smoke_pos, effect::enemy::HIPDROP_MAX * 2);
		smoke_eff->SetDamageEffect(240, 240, -20, 140, 6, 15);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(smoke_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::AddDebufEffect() {
		_game.GetObjectServer()->GetPlayer()->Debuf();	// 自機のデバフ処理呼び出し
		auto world = _game.GetMapChips()->GetWorldPosition();
		Vector2 debuf_pos = { world.GetX(), world.GetY() };
		auto debuf = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::DEBUF, debuf_pos, effect::crow::DEBUF_MAX * 3);
		debuf->SetLoop(3);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(debuf), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::AddAngerEffect() {
		auto anger = std::make_unique<LoopEffect>(_game.GetGame(), effect::crow::AURA, _position, effect::crow::AURA_MAX * 2);
		anger->SetOwner(this);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(anger), effect::type::FORMER);
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

	bool CrowDoll::AngerOn() {
		if (_isAnger == true) return false;
		// 耐久力は半分まで減っているか？
		if (IsAnger() == IS_ANGER) {
			_isAnger = true;	// 怒り状態に突入する
			ModeChange(CrowState::DEBUF, enemy::crowdoll::CROW_IDOL);	// デバフ状態に遷移する
			AddAngerEffect();
			_muteki = 120;
			return true;
		}
		return false;
	}

	AABB CrowDoll::NowCollision(std::string key) {
		if (_cState != CrowState::DEATH) return _mainCollision;
		// 死んでいる場合は当たり判定を返す
		auto ite = _collisions.find(enemy::crowdoll::CROW_DOWN);
		return ite->second;
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
			PlaySe(enemy::crowdoll::SE_BLINK_ATTACK);
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

	std::pair<AABB, AABB> CrowDoll::BlinkVitalPart(Collision& col, int vital) {
		AABB leftBox = { {col.GetMax().GetX() - vital, col.GetMin().GetY()}, {col.GetMax().GetX(), col.GetMax().GetY()}, true};
		AABB RightBox = { {col.GetMax().GetX() + vital, col.GetMin().GetY()}, {col.GetMax().GetX(), col.GetMax().GetY()}, true};
		return std::make_pair(leftBox, RightBox);
	}

	bool CrowDoll::AttackBox(bool flag) {
		auto rush = _collisions.find(enemy::crowdoll::CROW_RUSH);
		if (rush->second.GetCollisionFlag() == flag) return false;
		rush->second.SetCollisionFlag() = flag;
#ifdef _DEBUG
		rush->second.SetDrawFlag() = flag;
#endif
		return true;
	}

	bool CrowDoll::DeathOn() {
		// 耐久値がある場合は生存している
		if (_life != 0) return false;
		_game.GetModeServer()->GetModeMain()->BossEnd();	// ボス戦を終了する
		ModeChange(CrowState::DEATH, enemy::crowdoll::CROW_DOWN);	// 死亡状態に移行する
		// 現在の地点に死亡エフェクトを発生させる
		auto death_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::DEATH, _position, effect::crow::DEATH_MAX * 3);
		PlaySe(enemy::crowdoll::SE_DEATH);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(death_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::DollsEnd() {
		// 魂を奪い去る
		auto enemys = _game.GetObjectServer()->GetEnemys();
		for (auto ite : enemys) {
			if (ite->GetEnemyType() == EnemyType::CROW_DOLL) continue;
			if (ite->IsEmpty() == true) continue;
			ite->SoulPop();
		}
		return true;
	}
}
