#include "ForeGround.h"
#include "ImageValue.h"
#include "Game.h"
#include "StageTransition.h"
#include <DxLib.h>

namespace inr {

	ForeGround::ForeGround(Game& game) : Image(game) {
		Init();
	}

	void ForeGround::Init() {
		_ivalue.clear();
	}

	bool ForeGround::SetForeGround(std::vector<ImageValue> ive) {
		_ivalue = ive;
	}

	void ForeGround::Draw() {
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_graphKey, 1);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}

	bool ForeGround::SetKey(const std::string key) {
		auto ckey = CheckKey(key);
		if (ckey == -1) return false;
		_graphKey = _ivalue[ckey].GraphKey();	// �摜�؂�ւ�
		_pos = _ivalue[ckey].Position();	// ���W�C��
		return true;
	}

	int ForeGround::CheckKey(const std::string key) {
		if (key == stage::STAGE_0) return 0;
		else if (key == stage::STAGE_1) return 1;
		else if (key == stage::STAGE_2) return 2;
		else if (key == stage::STAGE_2_1) return 3;
		else if (key == stage::STAGE_2_2) return 4;
		else if (key == stage::STAGE_3) return 5;
		else return -1;
	}

}