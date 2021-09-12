#include "Door.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "SoulSkin.h"

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
		_gType = GimmickType::DOOR;
		_color = -1;
		Init();
	}

	void Door::Init() {
		_switch = gimmick::OFF;
		_pal = 255;
	}

	void Door::Process() {
		if (_switch == gimmick::OFF) return;	// フラグがオンの場合は処理を終了する
		if (_pal == 0) return;
		_pal -= 5;
		if (_mainCollision.GetCollisionFlg() == true) _mainCollision.GetCollisionFlgB() = false;
		if (_pal < 0) _pal = 0;
	}

	void Door::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawDebugBox(_mainCollision);
	}

	void Door::SetParameter(Vector2 spwan, std::string key, int flag) {
		_position = spwan;
		_mainCollision = { _position, 20, 20, 10, 70, true };
		_divKey.first = key;
		_motionKey = { { _divKey.first, {25, 50}} };
		switch (flag) {
		case oscenario::gimmick::FLAG_FALSE:
			_switch = gimmick::OFF;
			break;
		case oscenario::gimmick::FLAG_TRUE:
			_switch = gimmick::ON;
			_pal = 0;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			break;
		default:
			_switch = gimmick::OFF;
			break;
		}
	}

	void Door::SwitchOn() {
		_switch = gimmick::ON;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}

	void Door::SwitchOff() {
		_switch = gimmick::OFF;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = true;
#endif
		_pal = 255;
	}

	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// 対象は接触しているか？
		if (_mainCollision.HitCheck(box) == false) return false;	// 衝突していない

		// 衝突している場合はどちら側からめり込んでいるかを算出する
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
		if (key == gimmick::door::KEY_DOOR_RED) _color = soul::RED;
		if (key == gimmick::door::KEY_DOOR_BLUE) _color = soul::BLUE;
		else _color = -1;
	}
}