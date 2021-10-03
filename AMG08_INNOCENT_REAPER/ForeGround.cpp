#include "ForeGround.h"
#include "ImageValue.h"
#include "Game.h"
#include "StageTransition.h"
#include "Loads.h"
#include <DxLib.h>

namespace inr {

	ForeGround::ForeGround(Game& game) : Image(game) {
		_ivalue = Loads::LoadForeGround1();
		_draw = false;
	}

	ForeGround::~ForeGround() {
		_ivalue.clear();
	}

	void ForeGround::Init() {
	}

	bool ForeGround::SetForeGround(std::vector<ImageValue> ive) {
		_ivalue = ive;
		return true;
	}

	void ForeGround::Draw() {
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_graphKey, 0);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}

	bool ForeGround::SetKey(const std::string key) {
		auto ckey = CheckKey(key);
		if (ckey == -1) return false;
		switch (ckey) {
		case 0:	// ステージ0
			_draw = true;
			_graphKey = _ivalue[ckey].GraphKey();	// 画像切り替え
			_pos = _ivalue[ckey].Position();	// 座標修正
			return true;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			_draw = false;
			_graphKey = _ivalue[ckey].GraphKey();	// 画像切り替え
			_pos = _ivalue[ckey].Position();	// 座標修正
			return true;
		default:
			return false;
		}
	}

	int ForeGround::CheckKey(const std::string key) {
		if (key == stage::STAGE_0) return 0;
		else if (key == stage::STAGE_T) return 1;
		else if (key == stage::STAGE_1) return 2;
		else if (key == stage::STAGE_2) return 3;
		else if (key == stage::STAGE_2_1) return 4;
		else if (key == stage::STAGE_2_2) return 5;
		else if (key == stage::STAGE_3) return 6;
		else return -1;
	}

}