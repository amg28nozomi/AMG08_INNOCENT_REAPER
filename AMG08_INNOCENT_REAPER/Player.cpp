#include "Player.h"
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "MapChips.h"
#include <DxLib.h>
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

	// 与えるアクションの当たり判定
	constexpr auto GIVE_WIDTH1 = 60;
	constexpr auto GIVE_WIDTH2 = -10;
	constexpr auto GIVE_HEIGHT1 = -30;
	constexpr auto GIVE_HEIGHT2 = 70;

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
		_direction = false;
		_changeGraph = true;
		_jumpPower = 0;
		_position = { START_POSITION_X, START_POSITION_Y};
		_divKey = std::make_pair(PKEY_IDOL, key::SOUND_NUM);
		_moveVector = { 0, 0 };
		// _moveVector = std::make_pair(0, 0);
		_mainCollision = { _position, (PLAYER_WIDTH / 2) - BF_WIDTH, PLAYER_WIDTH / 2, (PLAYER_HIGHT / 2) - BF_HEIGHT_MIN, (PLAYER_HIGHT / 2) + BF_HEIGHT_MAX };
		//(_position, PLAYER_WIDTH / 2, PLAYER_HIGHT / 2);
		Init();
	}

	Player::~Player() {
		_motionKey.clear();
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
		// 入力情報を取得
		auto leverLR = _game.GetLeverLR();
		auto key = _game.GetTrgKey();

		Move(leverLR); // 移動処理
		Action(key); // アクション
		Dash();	// ダッシュ
		Jump(); // ジャンプ処理

		// 位置座標の更新
		PositionUpdate();
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
		/*auto dx = _position.GetX();
		auto dy = _position.GetY();
		Vector2 minV(dx - PLAYER_WIDTH / 2, dy + PLAYER_HIGHT / 2);
		Vector2 maxV(dx + PLAYER_WIDTH / 2, dy - PLAYER_HIGHT / 2);
		AABB debugAABB(minV, maxV);

		debugAABB.DrawBox();*/
		DebugInfo();
		ObjectBase::DrawDebugBox(_mainCollision);
		DrawFormatString(0, 100, GetColor(255, 0, 0), "maincollision（minX:%d, minY:%d）\n", _mainCollision.GetMin().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 125, GetColor(255, 0, 0), "maincollision（maxX:%d, maxY:%d）\n", _mainCollision.GetMax().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 150, GetColor(255, 0, 0), "pos.x = %d\n", _position.IntX());
		DrawFormatString(0, 175, GetColor(255, 0, 0), "pos.y = %d\n", _position.IntY());
		DrawFormatString(0, 200, GetColor(255, 0, 0), "stand = %d\n", _stand);
		DrawFormatString(0, 225, GetColor(255, 0, 0), "_gravity = %d\n", _gravity);
#endif

		// アニメーションが終わっていない場合はカウントを増やす
		if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		else AnimationInit();	// カウント初期化
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
				if (_aState == ActionState::ROB || _aState == ActionState::GIVE) break;
				Rob(x, y); //　奪うアクション実行
				break;
			case PAD_INPUT_2:	// Yボタンが押された場合、「魂を与える」
				if (_aState == ActionState::GIVE || _aState == ActionState::ROB) break;
				Give(x, y);		// 与えるアクション実行
				break;
			//case PAD_INPUT_3:	// Aボタンが押された場合、「ジャンプ」
			//	// 
			//	//if (_aState != ActionState::IDOL && _aState != ActionState::MOVE || _aState == ActionState::JUMP) break;
			//	Jump();
			//	break;
			case PAD_INPUT_5:	// L1が押された場合、「魂を切り替える」
				break;
			case PAD_INPUT_6:	// R1が押された場合、「ダッシュ」
				if (_aState == ActionState::HIT) break;
				InputDash(x);
				break;
			}
		}
		// アイドル状態以外で、アニメーションが終わってない場合

		if(_input == false){
			if (_aState != ActionState::IDOL && _stand && _changeGraph != true) {
				if (_aState == ActionState::FALL) {
					auto sound1 = SoundResearch(key::SOUND_PLAYER_FALL);
					auto soundType = se::SoundServer::GetPlayType(_divKey.second);
					PlaySoundMem(sound1, soundType);
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
					_aCount = 0;
				}
				if (!_speed && _aCount == 0) {
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
				}
			}
			// 
			if (_aState == ActionState::JUMP) {
				if (0 <= _gravity) {
					_changeGraph = true;
					_aState = ActionState::FALL;
					_divKey.first = PKEY_FALL;

				}
			}
		}
		return false;
	}

	void Player::Move(int lever) {
		// 入力可能か？
		if (_input != true) {
			// 状態がアイドル、またはモーブの時だけ移動処理を行う。
			if (_input == false) {
				if (lever < -10) _direction = PL_LEFT;
				else if (10 < lever) _direction = PL_RIGHT;
			}

			if (_aState == ActionState::IDOL || _aState == ActionState::MOVE) {
				// 入力情報がある場合
				if (lever < -100 || 100 < lever) {
					// moveではない時、キーと状態を更新
					if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
						_changeGraph = true;
						_aState = ActionState::MOVE;
						_divKey.first = PKEY_RUN;
					}
					// _direction = ((0 < lever) ? false : true);
					/* _direction = ((0 < lever) ? true : false);
					_speed = (lever * MAX_SPPED) / 1000;
					Vector2 moveVector = { 1 * _speed, 0 * _speed };
					_position = _position + moveVector; */
					// SEの管理
					if (_aCount % GetSoundFrame(_divKey.first) == 0) {
						auto sound1 = SoundResearch(key::SOUND_PLAYER_RUN1);
						auto soundType = se::SoundServer::GetPlayType(_divKey.second);
						PlaySoundMem(sound1, soundType);
					}
					// return;
					// 立っていてかつ入力がない場合
				}
				else if (_stand && _aState == ActionState::MOVE) {
					_changeGraph = true;
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
					_speed = 0;
					return;
				}
			}
			// 座標変更
			_speed = (lever * MAX_SPPED) / 1000;
			// 移動距離算出
			/*double vectory = 0;
			if (JUMP_Y < _moveVector.GetY()) {
				_moveVector.second -= JUMP_Y;
				vectory = -JUMP_Y;
			}*/
			// 単位ベクトル
			_moveVector.GetPX() = 1.0 * _speed;
			// Vector2 moveVector = { 1 * _speed, vectory };
			// _position = _position + moveVector;
			_speed = 0;
		}
	}

	void Player::InputDash(double x) {
		if (!_dashInterval) {
			// ダッシュ状態ではない場合、各種初期化処理を実行
			if (_aState != ActionState::DASH) {
				_aState = ActionState::DASH;
				_divKey.first = PKEY_DASH;
				// auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
				// PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
				// ダッシュアクション後の座標を割り出す（敵 or マップチップに接触した場合はこの限りではない）
				(_direction == PL_LEFT) ? _lastX = x - DASH_MAX : _lastX = x + DASH_MAX;
				_changeGraph = true;
				_input = true;	// 他アクションの入力を停止する
			}
		}
	}

	// ダッシュ処理
	void Player::Dash() {
		// インターバルがある場合は減らす
		if (0 < _dashInterval) --_dashInterval;
		// ダッシュ状態ではない場合、処理を中断
		if (_input != true) return;

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
			_input = false;
			_dashInterval = DASH_INTERVAL;
			_changeGraph = true;
			// 立っているかどうかで次のモーションを判定
			if (_stand) {
				_aState = ActionState::IDOL;
				_divKey.first = PKEY_IDOL;
				return;
			} else {
				_aState = ActionState::FALL;
				_divKey.first = PKEY_FALL;
			}
		}

	}

	void Player::Jump() {
		// 落下状態ではないとき
		if (_stand) {
			// 入力を受け付けているか？
			if (_input != true) {
				auto pressKey = _game.GetKey();
				if (pressKey & PAD_INPUT_3) {
					// 溜めカウンタを増やす
					_jumpPower += 1;
					// 溜めカウンタがマックスではない場合、処理から抜ける
					if (_jumpPower < JUMP_MAX) {
						return;
					}
				}
				// 押下情報を取得
				// ジャンプの値がある場合
				if (0 < _jumpPower) {
					// Aキーの入力がない場合、ジャンプを実行
					_aState = ActionState::JUMP;
					_divKey.first = PKEY_JUMP;
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
		}
		_jumpPower = 0;
	}

	void Player::Rob(double x, double y) {
		if (_input != true) {
			_aState = ActionState::ROB;
			// キー情報が違う時、キー情報を更新
			if (_divKey.first != PKEY_ROB) {
				_divKey.first = PKEY_ROB;
				// SE読み込み
				auto sound1 = SoundResearch(key::SOUND_PLAYER_ROB);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));
#ifdef _DEBUG
				// 当たり判定の設定
				auto red = GetColor(255, 0, 0);
#endif
				// 当たり判定の設定（後程修正）
				// 以下はno処理
				//double minX, minY, maxX, maxY;
				//if (_direction) {
				//	// minX = minX - ROB_WIDTH;
				//	minX = x + PLAYER_WIDTH / 2;
				//	minY = y - PLAYER_HIGHT / 2;
				//	// maxX = (x + PLAYER_WIDTH / 2) + ROB_WIDTH;
				//	maxX = minX + ROB_WIDTH;
				//	maxY = y + PLAYER_HIGHT / 2;
				//}
				//else {
				//	minX = (x - PLAYER_WIDTH / 2) - ROB_WIDTH;
				//	minY = y - PLAYER_HIGHT / 2;
				//	maxX = minX + ROB_WIDTH;
				//	maxY = y + PLAYER_HIGHT / 2;
				//}

				//Vector2 amin(minX, minY);
				//Vector2 amax(maxX, maxY);

				auto it = _collisions.find(PKEY_ROB);
				// it->second.SetVector(amin, amax);
				it->second.GetbDrawFlg() = true;

			}
			_aCount = 0;
			_changeGraph = true;	// 状態遷移フラグオン
		}
	}

	void Player::Give(double x, double y) {

		if (_input != true) {
			_aState = ActionState::GIVE;
			if (_divKey.first != PKEY_GIVE) {
				_divKey.first = PKEY_GIVE;
				// SE読み込み
				auto sound1 = SoundResearch(key::SOUND_PLAYER_GIVE);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

				auto it = _collisions.find(PKEY_GIVE);
				it->second.GetbDrawFlg() = true;

			}
			_aCount = 0;
			_changeGraph = true;	// 状態遷移フラグオン
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
	}


	// デバッグ用処理（変数の表示・当たり判定の描画等）
#ifdef _DEBUG
	void Player::DebugInfo() {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "ActionStatet : %d\n", _aState);
		DrawFormatString(0, 25, GetColor(255, 255, 255), "Animation : %d\n", _aCount);


		/*auto minX = box.GetMin().IntX();
		auto minY = box.GetMin().IntY();
		auto maxX = box.GetMax().IntX();
		auto maxY = box.GetMax().IntY();
		auto boxColor = GetColor(255, 255, 255);

		DrawBox(minX, minY, maxX, maxY, boxColor, FALSE);*/
		// DrawBox(box.GetMin().IntX(), box.GetMin().IntY(), box.GetMax().IntX(), box.GetMax().IntX(),
				// GetColor(255, 255, 255), FALSE);
	}
#endif
}