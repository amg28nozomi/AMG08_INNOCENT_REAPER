#include "Player.h"
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "MapChips.h"
#include <DxLib.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace {
#ifdef _RELEASE
	constexpr auto START_POSITION_X = inr::WINDOW_H / 2;
	constexpr auto START_POSITION_Y = inr::WINDOW_W / 2;
#endif
#ifdef _DEBUG
	constexpr auto START_POSITION_X = 100;
	constexpr auto START_POSITION_Y = 1900;
#endif

	// constexpr auto ROB

	// constexpr auto PLAYER_HIGHT = 250; // 300
	// constexpr auto PLAYER_WIDTH = 160; // 400

	constexpr auto MAX_SPPED = 6;

	constexpr auto BF_HEIGHT_MIN = 10;	// ボックス座標修正用
	constexpr auto BF_HEIGHT_MAX = 20;
	constexpr auto BF_WIDTH = 10;

	// アクションのフレーム数
	constexpr auto ACTION_MAX = 3;
	
	// SEのフレーム数
	constexpr auto SE_RUN1 = 50;	// 移動SEのフレーム数 


	// 奪うアクションの当たり判定
	constexpr auto ROB_WIDTH1 = 20 + 50; //-75;
	constexpr auto ROB_WIDTH2 = -20;
	constexpr auto ROB_HEIGHT1 = -20;
	constexpr auto ROB_HEIGHT2 = 60;
	// 奪うアクションの判定フレーム数
	constexpr auto JUDGE_MIN = -1;
	constexpr auto ROB_JUDGEMENT = 8 * 4;// judgement

	// 与えるアクションの当たり判定
	constexpr auto GIVE_WIDTH1 = 60;
	constexpr auto GIVE_WIDTH2 = -10;
	constexpr auto GIVE_HEIGHT1 = -30;
	constexpr auto GIVE_HEIGHT2 = 70;
	// 与えるアクションの判定フレーム数
	constexpr auto GIVE_JUDGEMENT = 10 * 4;

	// ジャンプアクション
	constexpr auto JUMP_VECTOR = 1;	// ジャンプの移動ベクトル
	constexpr auto JUMP_MAX = 15;
	constexpr auto JUMP_Y = 5;
	
	// ダッシュアクション関連
	constexpr auto DASH_INTERVAL = 60;	// ダッシュモーション後のインターバル時間
	constexpr auto DASH_TIME = 50 / 4;	// ダッシュアクションが完了するまでの時間
	constexpr auto DASH_MAX = 280;	// ダッシュモーションの最大移動距離
	constexpr auto DASH_VEC = DASH_MAX / DASH_TIME;	// 移動ベクトル

	constexpr auto DASH_WIDTH1 = 40;
	constexpr auto DASH_WIDTH2 = 30;
	constexpr auto DASH_HEIGHT1 = 10;
	constexpr auto DASH_HEIGHT2 = 70;


	// 各種モーションの画像数
	constexpr auto PF_IDOL = 13;
	constexpr auto PF_RUN = 9;
	constexpr auto PF_DASH = 13;
	constexpr auto PF_JUMP = 6;
	constexpr auto PF_FALL = 10;
	constexpr auto PF_ROB = 13;
	constexpr auto PF_GIVE = 16;
	constexpr auto PF_HIT = 7;

	// 描画切り替えまでに必要なフレーム数
	constexpr auto MF_INTERVAL = 4;

	// 再生時間（Player Motion Frame）
	constexpr auto PMF_IDOL = PF_IDOL * MF_INTERVAL;
	constexpr auto PMF_RUN = PF_RUN * MF_INTERVAL;
	constexpr auto PMF_DASH = PF_DASH * MF_INTERVAL;
	constexpr auto PMF_JUMP = PF_JUMP * MF_INTERVAL;
	constexpr auto PMF_FALL = PF_FALL * MF_INTERVAL;
	constexpr auto PMF_ROB = PF_ROB * MF_INTERVAL;
	constexpr auto PMF_GIVE = PF_GIVE * MF_INTERVAL;
	constexpr auto PMF_HIT = PF_HIT * MF_INTERVAL;
}

namespace inr {

	Player::Player(Game& game) : ObjectBase::ObjectBase(game) {
		_type = ObjectType::PLAYER;
		_aState = ActionState::IDOL;

		_aCount = 0;
		_aFrame = 0;
		_sounds = 0;

		_judegFrame = 0;

		_direction = false;
		_changeGraph = true;
		_input = true;
		_jumpPower = 0;
		_position = { START_POSITION_X, START_POSITION_Y};
		_divKey = std::make_pair(PKEY_IDOL, key::SOUND_NUM);
		_moveVector = { 0, 0 };
		// _moveVector = std::make_pair(0, 0);
		_mainCollision = { _position, (PLAYER_WIDTH / 2) - BF_WIDTH, PLAYER_WIDTH / 2, (PLAYER_HIGHT / 2) - BF_HEIGHT_MIN, (PLAYER_HIGHT / 2) + BF_HEIGHT_MAX , true};
		//(_position, PLAYER_WIDTH / 2, PLAYER_HIGHT / 2);
		Init();
	}

	Player::~Player() {
		_motionKey.clear();
		_collisions.clear();
	}

	void Player::Init() {
		// キー名　first:アニメーションの総フレーム数、second:SEの再生フレーム数
		_motionKey = {{PKEY_IDOL, {PMF_IDOL, SE_NUM}}, 
					{PKEY_RUN, {PMF_RUN, SE_RUN1}}, 
					{PKEY_DASH, {PMF_DASH, 50}},
					{PKEY_JUMP, {PMF_JUMP, 50}},
					{PKEY_FALL, {PMF_FALL, 50}},
					{PKEY_ROB, {PMF_ROB, 10}}, 
					{PKEY_GIVE, {PMF_GIVE, 10}},
					{PKEY_HIT, {PMF_HIT, 50}},
		};

		auto x = _position.GetX();
		auto y = _position.GetY();


		AABB dashBox = { _position, DASH_WIDTH1, DASH_WIDTH2, DASH_HEIGHT1, DASH_HEIGHT2 };
		AABB robBox = { _position, ROB_WIDTH1, ROB_WIDTH2, ROB_HEIGHT1, ROB_HEIGHT2 };
		AABB giveBox = { _position, GIVE_WIDTH1, GIVE_WIDTH2, GIVE_HEIGHT1, GIVE_HEIGHT2 };
		
		_collisions = { {PKEY_ROB, {robBox}},
						{PKEY_GIVE, {giveBox}},
						{PKEY_DASH, {dashBox}} };
}

	void Player::Process() {
		ObjectBase::Process();
		AnimationCount();
		// 入力情報を取得
		auto leverLR = _game.GetLeverLR();
		auto key = _game.GetTrgKey();

		Move(leverLR); // 移動処理
		Action(key); // アクション
		Dash();	// ダッシュ
		Jump(); // ジャンプ処理

		// 位置座標の更新
		PositionUpdate();

		// 各種衝突処理
		auto&& objs = _game.GetObjectServer()->GetObjects();

		auto cBox = _collisions.find(_divKey.first);
		if (cBox != _collisions.end()) {
			for (auto&& obj : objs) {
				if (obj->GetType() != ObjectType::ENEMY) continue;
				obj->CollisionHit(_divKey.first, cBox->second, _direction);
			}
		}

		_game.GetMapChips()->WorldUpdate(_position);
	}

	void Player::Draw() {
		 Vector2 xy = _position;
		 _game.GetMapChips()->Clamp(xy);
		 auto x = xy.IntX();
		 auto y = xy.IntY();

		DrawFormatString(1500, 100, GetColor(0, 255, 0), "描画座標x = %d", x);
		DrawFormatString(1500, 150, GetColor(0, 255, 0), "描画座標y = %d", y);

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

#ifdef _DEBUG
		DebugInfo();
#endif
	}

	void Player::StateUpdate() {
		// 各種モーションに合わせた修正
		switch (_aState) {
		// 移動時
		case ActionState::MOVE:
			/*if (!_stand) {
				ChangeState(ActionState::FALL, PKEY_FALL);
			}*/
			
			return;
		// ジャンプ時
		case ActionState::JUMP:
			// 加速値がプラスの場合
			if (0 < _gravity) {
				_changeGraph = true;
				_aState = ActionState::FALL;
				_divKey.first = PKEY_FALL;
			}
			return;
		// 落下時
		case ActionState::FALL:
			// 立っていてる場合
			if (_stand && _gravity == 0) {
				//// 着地音を鳴らす
				auto land = SoundResearch(key::SOUND_PLAYER_FALL);
				auto soundType = se::SoundServer::GetPlayType(_divKey.second);
				PlaySoundMem(land, soundType);
				ChangeState(ActionState::IDOL, PKEY_IDOL);
			}
			return;
		// 奪うアクション時
		case ActionState::ROB:
			// 描画更新がある場合は処理から抜ける
			if (_changeGraph == true) break;
			if(0 <= _judegFrame) --_judegFrame;	// カウンタ減少
			// 判定カウンタはゼロになったか？
			if (_judegFrame == 0) {
				auto it = _collisions.find(_divKey.first);
				// 当たり判定を止める
				it->second.GetCollisionFlgB() = false;
#ifdef _DEBUG
				it->second.GetbDrawFlg() = false;
#endif
			} 
			// アニメーションカウンタはマックスになったか？
			if (AnimationCountMax()) {
				ChangeState(ActionState::IDOL, PKEY_IDOL);
				_input = true;	// 入力を受け付ける
			}
			break;
		case ActionState::GIVE:
			if (_changeGraph == true) break;
			if (0 <= _judegFrame) --_judegFrame;	// カウンタ減少
			// 判定カウンタはゼロになったか？
			if (_judegFrame == 0) {
				auto it = _collisions.find(_divKey.first);
				// 当たり判定を止める
				it->second.GetCollisionFlgB() = false;
#ifdef _DEBUG
				it->second.GetbDrawFlg() = false;
#endif
			}
			// アニメーションカウンタはマックスになったか？
			if (AnimationCountMax()) {
				ChangeState(ActionState::IDOL, PKEY_IDOL);
				_input = true;	// 入力を受け付ける
			}
			break;
		default:
			return;
		}
	}

	bool Player::Action(int key) {
		// 前フレームの情報
		auto beforeState = _aState;
		auto x = _position.GetX();
		auto y = _position.GetY();

		// keyに入力情報がある場合
		if (key) {
			// 入力情報に応じた処理を実行
			switch (key) {
			case PAD_INPUT_1:	// Xボタンが押された場合、「魂を奪う」
				// 前フレームの状態と同じ場合は処理から抜ける
				// if (_aState == ActionState::ROB || _aState == ActionState::GIVE) break;
				Rob(x, y); //　奪うアクション実行
				break;
			case PAD_INPUT_2:	// Yボタンが押された場合、「魂を与える」
				if (_aState == ActionState::GIVE || _aState == ActionState::ROB) break;
				Give(x, y);		// 与えるアクション実行
				break;
			case PAD_INPUT_3:	// Aボタンが押された場合、「ジャンプ」
				InputJump();
			//	// 
			//	//if (_aState != ActionState::IDOL && _aState != ActionState::MOVE || _aState == ActionState::JUMP) break;
			//	Jump();
				break;
			case PAD_INPUT_5:	// L1が押された場合、「魂を切り替える」
				break;
			case PAD_INPUT_6:	// R1が押された場合、「ダッシュ」
				if (_aState == ActionState::HIT) break;
				InputDash(x);
				break;
			}
		}
		// アイドル状態以外で、アニメーションが終わってない場合
		StateUpdate();
		
				/*if (!_speed && _aCount == 0) {
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
				}*/

		return false;
	}

	void Player::Move(int lever) {
		// 入力可能か？
		if (_input == true) {
			// 状態がアイドル、またはモーブの時だけ移動処理を行う。
				if (lever < -10) _direction = PL_LEFT;
				else if (10 < lever) _direction = PL_RIGHT;

			if (_aState != ActionState::FALL && _aState == ActionState::IDOL || _aState == ActionState::MOVE) {
				// 入力情報がある場合
				if (lever < -100 || 100 < lever) {
					// moveではない時、キーと状態を更新
					if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
						ChangeState(ActionState::MOVE, PKEY_RUN);
					}
					// SEの管理
					if (_aCount % GetSoundFrame(_divKey.first) == 0) {
						auto sound1 = SoundResearch(key::SOUND_PLAYER_RUN1);
						auto soundType = se::SoundServer::GetPlayType(_divKey.second);
						PlaySoundMem(sound1, soundType);
					}
					// return;
					// 立っていてかつ入力がない場合
				}
				else if (_aState == ActionState::MOVE) {
					switch (_stand) {
					case true:	// 立っている場合
						ChangeState(ActionState::IDOL, PKEY_IDOL);
						_speed = 0;
						break;
					//case false:	// 落下状態の場合
					//	ChangeState(ActionState::FALL, PKEY_FALL);
					//	_speed = 0;
					//	break;
					}
					return;
				}
			}
			// 座標変更
			_speed = (lever * MAX_SPPED) / 1000;
			// 移動ベクトル代入
			_moveVector.GetPX() = 1.0 * _speed;
			_speed = 0;
		}
	}

	void Player::InputDash(double x) {
		// 入力可能状態かつ、インターバル中ではないか？
		if (_input == true && !_dashInterval) {
			// ダッシュ状態ではない場合、各種初期化処理を実行
			if (_aState != ActionState::DASH) {
				ChangeState(ActionState::DASH, PKEY_DASH);
				// auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
				// PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
				// ダッシュアクション後の座標を割り出す（敵 or マップチップに接触した場合はこの限りではない）
				(_direction == PL_LEFT) ? _lastX = x - DASH_MAX : _lastX = x + DASH_MAX;
				_input = false;	// 他アクションの入力を停止する
			}
		}
	}

	void Player::InputJump() {
		// 入力可能状態かつ、立っている場合のみ実行可能
		if (_input == true && _stand) {
			_jumpPower += 1;	// 溜めカウンタを増やす
		}
	}

	// ダッシュ処理
	void Player::Dash() {
		// インターバルがある場合は減らす
		if (0 < _dashInterval) --_dashInterval;
		// ダッシュ状態ではない場合、処理を中断
		if (_input == true) return;

		if (_aState == ActionState::DASH) {
			double dashVec;	// 移動ベクトル
		// 向いている向きに応じて代入するベクトルを切り替え
			(_direction == PL_LEFT) ? dashVec = -DASH_VEC : dashVec = DASH_VEC;
			_moveVector.GetPX() = dashVec;

			auto nextPos = _position.GetX() + dashVec;	// 移動後の座標を取得
			_gravity = 0;	// ダッシュ中は重力処理無効
			bool moved;
			(_direction == PL_LEFT) ? moved = nextPos - _lastX <= 0 : moved = _lastX - nextPos <= 0;

			// ダッシュ処理は完了したかどうか？
			if (moved) {
				_input = true;
				_dashInterval = DASH_INTERVAL;
				// 立っているかどうかで次のモーションを判定
				if (_stand) {
					ChangeState(ActionState::IDOL, PKEY_IDOL);
					return;
				}
				else {
					ChangeState(ActionState::FALL, PKEY_FALL);
				}
			}
		}
	}

	void Player::Jump() {
		if (_input == true && _stand) {
			// 溜めはあるか？
			if (1 <= _jumpPower) {
				auto pressKey = _game.GetKey();
				if (pressKey & PAD_INPUT_3) {
					// 溜めカウンタを増やす
					_jumpPower += 1;
					// 溜めカウンタがマックスではない場合、処理から抜ける
					if (_jumpPower < JUMP_MAX) {
						return;
					}
				}
				// Aキーの入力がない場合、ジャンプを実行
				ChangeState(ActionState::JUMP, PKEY_JUMP);
				auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
				PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
				// 飛距離を算出
				auto jumpPower = JUMP_VECTOR * (1.0 + _jumpPower);
				// 飛距離が最大値を超えた場合は修正
				if (JUMP_MAX < jumpPower) jumpPower = JUMP_MAX;
				// ジャンプの飛距離を登録
				// この値は地面に触れた or 天井に接触した場合、0にする。
				_gravity = -jumpPower;
			}
		}
		if(_jumpPower) _jumpPower = 0;
	}

	void Player::Rob(double x, double y) {
		if (_input == true) {
			_aState = ActionState::ROB;
			// キー情報が違う時、キー情報を更新
			if (_divKey.first != PKEY_ROB) {
				_divKey.first = PKEY_ROB;
				// SE読み込み
				auto sound1 = SoundResearch(key::SOUND_PLAYER_ROB);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

				auto it = _collisions.find(PKEY_ROB);
				// it->second.SetVector(amin, amax);
				it->second.GetCollisionFlgB() = true;	// 判定オン
#ifdef _DEBUG
				it->second.GetbDrawFlg() = true;
#endif

			}
			_changeGraph = true;	// 状態遷移フラグオン
			_input = false; // 入力を受け付けなくする
			_judegFrame = ROB_JUDGEMENT;	// 判定カウンタ
		}
	}

	void Player::Give(double x, double y) {

		if (_input == true) {
			_aState = ActionState::GIVE;
			if (_divKey.first != PKEY_GIVE) {
				_divKey.first = PKEY_GIVE;
				// SE読み込み
				auto sound1 = SoundResearch(key::SOUND_PLAYER_GIVE);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

				auto it = _collisions.find(PKEY_GIVE);
				it->second.GetCollisionFlgB() = true;	// 判定オン
#ifdef _DEBUG
				it->second.GetbDrawFlg() = true;
#endif

			}
			_changeGraph = true;	// 状態遷移フラグオン
			_input = false; // 入力を受け付けなくする
			_judegFrame = ROB_JUDGEMENT;	// 判定カウンタ
		}
	}

	void Player::ChangeSoul() {
	
	}

	void Player::Damage(){
	
	}

	bool Player::Dead() {
		return false; 
	}

	void Player::AnimationInit() { 
		if (_aState != ActionState::IDOL && _aState !=ActionState::MOVE && _aState != ActionState::JUMP && _aState!= ActionState::FALL && _aState!=ActionState::DASH) {
			// 以下のコードは修正予定
			// フラグをオフにする
			auto it = _collisions.find(_divKey.first);
			it->second.GetbDrawFlg() = false;
		}
		_aCount = 0;
	}

	AABB Player::GetAABB() {
		auto dx = _position.GetX();
		auto dy = _position.GetY();
		Vector2 minV(dx - PLAYER_WIDTH / 2, dy + PLAYER_HIGHT / 2);
		Vector2 maxV(dx + PLAYER_WIDTH / 2, dy - PLAYER_HIGHT / 2);
		AABB playerAABB(minV, maxV);

		return playerAABB;
	}

	// 位置座標更新
	void Player::PositionUpdate() {
		// 移動ベクトルYに加速度を代入
		_moveVector.GetPY() = _gravity;
		// マップチップにめり込んでいる場合は座標を修正
		_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector);
		_position = _position + _moveVector;	// 位置座標を更新

		// 各種当たり判定の更新
		auto it = _collisions.find(_divKey.first);
		if (it != _collisions.end()) {
			it->second.Update(_position, _direction);
		}

		if (_aState == ActionState::DASH) {
			_mainCollision.Swap(it->second);
		} else {
			_mainCollision.Update(_position, _direction);
		}
		// 移動ベクトル初期化
		_moveVector = { 0, 0 };
	}

	void Player::ChangeState(Player::ActionState nextState, std::string key) {
		_aState = nextState;
		_divKey.first = key;
		_changeGraph = true;
	}


	// デバッグ用処理（変数の表示・当たり判定の描画等）
#ifdef _DEBUG
	void Player::DebugInfo() {
		DrawFormatString(0, 0, GetColor(255, 0, 255), "ActionStatet : %d\n", _aState);
		DrawFormatString(0, 25, GetColor(255, 0, 255), "Animation : %d\n", _aCount);
		ObjectBase::DrawDebugBox(_mainCollision);
		DrawFormatString(0, 100, GetColor(255, 0, 0), "maincollision（minX:%d, minY:%d）\n", _mainCollision.GetMin().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 125, GetColor(255, 0, 0), "maincollision（maxX:%d, maxY:%d）\n", _mainCollision.GetMax().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 150, GetColor(255, 0, 0), "pos.x = %d\n", _position.IntX());
		DrawFormatString(0, 175, GetColor(255, 0, 0), "pos.y = %d\n", _position.IntY());
		DrawFormatString(0, 200, GetColor(255, 0, 0), "stand = %d\n", _stand);
		DrawFormatString(0, 225, GetColor(255, 0, 0), "_gravity = %d\n", _gravity);
	}
#endif
}