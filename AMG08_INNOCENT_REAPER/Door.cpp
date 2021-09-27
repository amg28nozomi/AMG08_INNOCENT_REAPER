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
		_gType = GimmickType::DOOR;
		_color = -1;
		Init();
	}

	void Door::Init() {
		_switch = gimmick::OFF;
		_ismove = false;
		_moves = {};
	}

	void Door::Process() {
		// if (_switch == gimmick::OFF && _ismove == false) return;	// フラグがオンの場合は処理を終了する

		if (_ismove == false) return;
		switch (_switch) {
		case false:
			_moves.GetPY() = DOOR_VECTOR;
			_position = _position + _moves;
			if (_normalY <= _position.GetY()) _ismove = false;
			break;
		case true:
			_moves.GetPY() = -DOOR_VECTOR;
			_position = _position + _moves;
			if (_position.GetY() <= _normalY - OPEN_MAX) _ismove = false;
			break;
		}
		_mainCollision.Update(_position, false);

		_moves = {};
	}

	void Door::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph = graph::ResourceServer::GetHandles(_divKey.first, 0);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif

	}

	void Door::SetParameter(Vector2 spwan, std::string key, int flag) {
		_divKey.first = key;
		_normalY = spwan.GetY();
		SetColor(key);
		_motionKey = { { _divKey.first, {25, 50}} };
		_ismove = false;

		bool colf;
		switch (flag) {
		case oscenario::gimmick::FLAG_FALSE:
			colf = true;	// 当たり判定を元に戻す
			_position = spwan;
			_switch = gimmick::OFF;
			break;
		case oscenario::gimmick::FLAG_TRUE:
			_position = { spwan.GetX(), _normalY - OPEN_MAX };
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			colf = false;	// 当たり判定を元に戻す
			break;
		default:
			_position = spwan;
			_switch = gimmick::OFF;
			colf = true;
			break;
		}
		_mainCollision = { _position, 20, 20, 10, 70, colf };
	}

	void Door::SwitchOn() {
		_switch = gimmick::ON;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
		_mainCollision.GetCollisionFlgB() = false;	// 当たり判定を元に戻す
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}

	void Door::SwitchOff() {
		_switch = gimmick::OFF;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = true;
#endif
		_mainCollision.GetCollisionFlgB() = true;	// 当たり判定を元に戻す
	}

	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// 対象は接触しているか？
		if (_mainCollision.HitCheck(box) == false) return false;	// 衝突していない


		if (move.GetX() < 0) {
			// 左から接触している
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		}
		else if (0 < move.GetX()){
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;

		// 反転フラグは立っているか？
		//switch (changedirection) {
		//case true:
		//	// 右から接触した場合に左方向に入力がある場合
		//	if (move.GetX() < 0) {
		//		double fix = _mainCollision.GetWidthMin() + box.GetWidthMin();
		//		pos.GetPX() = _position.GetX() - fix;
		//		move.GetPX() = 0;
		//	}
		//	else {
		//		// 左から接触している
		//		double fix = _mainCollision.GetWidthMin() + box.GetWidthMin();
		//		pos.GetPX() = _position.GetX() + fix;
		//		move.GetPX() = 0;
		//	}
		//	return true;
		//case false:
		//	// 衝突している場合はどちら側からめり込んでいるかを算出する
		//	if (move.GetX() < 0) {
		//		// 左から接触している
		//		pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMin();
		//		move.GetPX() = 0;
		//	}
		//	else {
		//		pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
		//		move.GetPX() = 0;
		//	}
		//	return true;
		//}
		//return true;
	}

	void Door::SetColor(std::string key) {
		// 色指定
		//if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		else if (key == gimmick::door::KEY_DOOR_BLUE) _color = static_cast<int>(soul::BLUE);
		else _color = -1;
	}

	void Door::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// オブジェクト情報の更新
		_divKey.first = gimmick::door::KEY_DOOR_BOSS;
		_position = objValue.Positions().at(0);
		_normalY = _position.GetY();
		_mainCollision = { _position, 20, 20, 10, 70, true };
		switch (_game.GetModeServer()->GetModeMain()->BossOpen()) {	// 扉は開かれているか？
		case true:	// 空いている場合
			_switch = gimmick::ON;
			_mainCollision.GetCollisionFlgB() = false;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			return;
		case false:	// 閉じている場合
			_switch = gimmick::OFF;
			_mainCollision.GetCollisionFlgB() = true;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = true;
#endif
			return;
		}
	}
}