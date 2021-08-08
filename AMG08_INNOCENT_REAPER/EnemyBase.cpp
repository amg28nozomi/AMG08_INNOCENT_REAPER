#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"

using std::string;

namespace inr {

	namespace {
		constexpr auto ESCAPE_MAX = 1000;
		constexpr auto ESCAPE_VECTOR = 80;	// 1秒間(60frame)で移動する距離

		constexpr auto FRAME = 60;
	}


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
		auto x = _position.IntX();
		auto y = _position.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				box->second.Draw(GetColor(255, 0, 0));
			}
		}
		//// アニメーションが終わっていない場合はカウントを増やす
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// カウンター初期化
		_mainCollision.DrawBox();
		_searchBox.DrawBox();
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
			_divKey.first = enemy::SOLDIER_ESCAPE;
			// 自身の向きによって移動距離を変更
			if (_direction) _actionX = -ESCAPE_MAX / FRAME;
			else _actionX = ESCAPE_MAX / FRAME;
		}
		// 向きによって加算・減算切り替え
		if (_direction) {
			_actionX -= ESCAPE_VECTOR / FRAME;
			_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
			return;
		}
		_actionX += ESCAPE_VECTOR / FRAME;
		_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
	}

	int EnemyBase::GetSize(const std::string& key) {
		// 要素検索
		auto it = _enemyKey.find(key);
		if (it == _enemyKey.end()) {
			// イテレータが見つからなった場合は-1を返す
#ifdef _DEBUG
			OutputDebugString("キーがヒットしませんでした。キー情報を確認してください。\n");
#endif
			return -1;
		}
		return it->second.first;
	}

	bool EnemyBase::GraphResearch(int* gh) {
		// フラグがオンの時、描画するグラフィックを切り替える
		if (_changeGraph) {
			_changeGraph = false;
			*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// 最初の要素を取得
			return true;
		}
		// グラフィックが切り替わる猶予フレームを算出
		auto interval = GetSize(_divKey.first) / graph::ResourceServer::GetAllNum(_divKey.first);
		// 何番目のアニメーションが呼び出されているか
		auto no = _aCount / interval % graph::ResourceServer::GetAllNum(_divKey.first);
		// グラフィックハンドルを読み込む
		*gh = graph::ResourceServer::GetHandles(_divKey.first, no);
		return false;
	}

	int EnemyBase::SoundResearch(const std::string& key) {
		_divKey.second = key;
		auto sound = se::SoundServer::GetSound(_divKey.second);
		return sound;
	}

	int EnemyBase::GetSoundFrame(const std::string& key) {
		// キー検索
		auto it = _enemyKey.find(key);
		// ヒットしなかった場合は-1を返す。
		if (it == _enemyKey.end()) {
			return -1;
		}
		// SEの再生フレーム時間を取り出して返す。
		auto soundFrame = it->second.second;
		return soundFrame;
	}
}