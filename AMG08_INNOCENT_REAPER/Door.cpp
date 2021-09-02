#include "Door.h"
#include "Game.h"
#include "ObjectServer.h"

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
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
		if (_mainCollision.GetCollisionFlg() == true) _mainCollision.GetCollisionFlgB() == false;
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
	}

	void Door::SetParameter(Vector2 spwan, std::string key) {
		_position = spwan;
		_mainCollision = { _position, 40, 40, 10, 70, true };
		_divKey.first = key;
	}

	void Door::SwitchOn() {
		_switch = gimmick::ON;
	}
}