#include "Door.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "SoulSkin.h"

#include "ModeServer.h"
#include "ModeMain.h"

namespace {
	constexpr auto OPEN_MAX = 200;
	constexpr auto ANIMATION_FRAME = 50;
	constexpr auto DOOR_VECTOR = OPEN_MAX / ANIMATION_FRAME;
}

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
		// 初期化
		_gType = GimmickType::DOOR;
		_color = -1;
		Init();
	}
	// 初期化
	void Door::Init() {
		// 各種初期化
		_switch = gimmick::OFF;
		_changeGraph = false;
		_ismove = false;
		_moves = {};
		_normalY = 0;
		// 各種キーの設定
		_motionKey = {
			{ gimmick::door::KEY_DOOR_LEVER, {14 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BLUE, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_RED, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BOSS, {1, 0}},
		};
	}
	// 更新
	void Door::Process() {
		// ドアの移動処理がある場合は中断
		if (DoorMove() == true) return;
		MotionCount();	// アニメーションの再生
	}
	// 描画
	void Door::Draw() {
		// 描画座標の算出
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// グラフィックハンドルの取得
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		// 当たり判定の描画
		DrawDebugBox(_mainCollision);
#endif
	}
	// オブジェクト情報の設定
	void Door::SetParameter(Vector2 spwan, std::string key, int flag) {
		// 各種登録
		_divKey.first = key;
		SetColor(key);	// 色の判別
		_position = spwan;
		_ismove = false;
		// 対応する扉の登録情報呼び出し
		auto ite = _motionKey.find(_divKey.first);

		bool colf;	// 当たり判定のフラグ
		// ギミックフラグに応じて、状態切り替え
		switch (flag) {
		case oscenario::gimmick::FLAG_FALSE:
			colf = true;	// 当たり判定を元に戻す
			_switch = gimmick::OFF;
			_aCount = 0;
			break;
		case oscenario::gimmick::FLAG_TRUE:
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			_aCount = ite->second.first - 1;
			colf = false;	// 当たり判定を元に戻す
			break;
		default:
			_switch = gimmick::OFF;
			_aCount = 0;
			colf = true;
			break;
		}
		// 当たり判定の修正
		_mainCollision = { _position, 20, 20, 10, 70, colf };
	}
	// 開閉フラグの起動
	void Door::SwitchOn() {
		_switch = gimmick::ON;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
		_mainCollision.SetCollisionFlag() = false;	// 当たり判定を元に戻す
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}
	// 開閉フラグの抑制
	void Door::SwitchOff() {
		auto sound = se::SoundServer::GetSound(gimmick::door::SE_CLOSE_DOOR);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

		_switch = gimmick::OFF;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = true;
#endif
		_mainCollision.SetCollisionFlag() = true;	// 当たり判定を元に戻す
	}
	// 押し出し処理
	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// 対象は接触しているか？
		if (_mainCollision.HitCheck(box) == false) return false;	// 衝突していない
		// 接触している場合、移動方向とは逆方向に押し出す
		if (move.GetX() < 0) {
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		} else if (0 < move.GetX()){
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;	// 衝突している
	}
	// ドアの移動処理
	bool Door::DoorMove() {
		// ボス扉ではない場合は処理を中断する
		if (_divKey.first != gimmick::door::KEY_DOOR_BOSS) return false;
		if (_ismove == false) return true;	// 移動フラグが立っていない場合も中断
		// フラグに応じて処理を切り替える
		switch (_switch) {
			// 扉を閉める
		case false:
			_moves.GetPY() = DOOR_VECTOR;
			_position = _position + _moves;
			if (_normalY <= _position.GetY()) {
				_position.GetPY() = _normalY;
				_ismove = false;
			}
			break;
			// 扉を開ける
		case true:
			_moves.GetPY() = -DOOR_VECTOR;
			_position = _position + _moves;
			if (_position.GetY() <= _normalY - OPEN_MAX) {
				_position.GetPY() = _normalY - OPEN_MAX;
				_ismove = false;
			}
			break;
		}
		// 各種更新
		_mainCollision.Update(_position, false);
		_moves = {};
	}
	// アニメーション処理
	bool Door::MotionCount() {
		// 移動フラグが立っている場合のみ再生を行う
		if (_ismove == false) return false;
		// 現在の開閉フラグに応じてアニメーション方法変更
		switch (_switch) {
		case gimmick::ON:
			if (AnimationCountMax() == true) {
				// アニメーションカウンタが上限に到達した場合、各種判定を偽にする
				if (_mainCollision.GetCollisionFlag() == true) {
					_mainCollision.SetCollisionFlag() = false;
#ifdef _DEBUG
					_mainCollision.SetDrawFlag() = false;
#endif
					_ismove = false;
				}
				return false;
			}
			++_aCount;
			return true;
		case gimmick::OFF:
			if (_aCount == 0) {
				// アニメーションカウンタが下限に到達した場合、各種判定を真にする
				if (_mainCollision.GetCollisionFlag() == false) {
					_mainCollision.SetCollisionFlag() = true;
#ifdef _DEBUG
					_mainCollision.SetDrawFlag() = true;
#endif
					_ismove = false;
				}
				return false;
			}
			--_aCount;
			return true;
		}
	}
	// 色の判定
	void Door::SetColor(std::string key) {
		// キーに応じて色を切り替える
		if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		else if (key == gimmick::door::KEY_DOOR_BLUE) _color = static_cast<int>(soul::BLUE);
		else _color = -1;
	}
	// オブジェクト情報の登録
	void Door::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// オブジェクト情報の更新
		_divKey.first = gimmick::door::KEY_DOOR_BOSS;	// 画像キーの設定
		_normalY = _oValue.Positions().at(0).GetY();	// 閉まっている際の描画座標

		bool colf = false;
		switch (_game.GetModeServer()->GetModeMain()->BossOpen()) {	// 扉は開かれているか？
		case true:	// 空いている場合
			colf = false;
			_position = { _oValue.Positions().at(0).GetX(), _normalY - OPEN_MAX };
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			break;
		case false:	// 閉じている場合
			colf = true;	// 当たり判定を元に戻す
			_position = _oValue.Positions().at(0);
			_switch = gimmick::OFF;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = true;
#endif
			break;
		}
		// 当たり判定の修正
		_mainCollision = { _position, 40, 60, 100, 130, colf };
	}
}