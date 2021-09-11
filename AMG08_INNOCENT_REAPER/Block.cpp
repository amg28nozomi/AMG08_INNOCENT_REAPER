#include "Block.h"
#include "Game.h"

namespace inr {

	Block::Block(Game& game) : GimmickBase(game) {
		_gType = GimmickType::BLOCK;
		_divKey = { gimmick::block::KEY_BLOCK, "" };
		_motionKey.clear();

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;	// ‰ó‚ê‚Ä‚¢‚È‚¢
	}

	void Block::Init() {

	}

	void Block::Process() {
		if (_break == gimmick::block::BRAKE_ON && _pal == 0) return;	// ˆ—‚ğI—¹‚·‚é
		if (_break != gimmick::block::BRAKE_ON) return;	// ‰ó‚ê‚Ä‚¢‚È‚¢ê‡‚àˆ—‚ğ”²‚¯‚é
		_pal -= 5;	// Œ¸Zˆ—‚ğs‚¤
		if (_pal < 0) _pal = 0;	// ¬‚³‚­‚È‚Á‚½ê‡‚Í’l‚ğC³
	}

	void Block::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawDebugBox(_mainCollision);
	}

	void Block::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];
		_mainCollision = { _position, 40, 40, 0, 80, true };
	}

	void Block::ObjValueUpdate() {
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);
	}


	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// ‘ÎÛ‚ÍÚG‚µ‚Ä‚¢‚é‚©H
		if (_mainCollision.HitCheck(box) == false) return false;	// Õ“Ë‚µ‚Ä‚¢‚È‚¢

		// Õ“Ë‚µ‚Ä‚¢‚éê‡‚Í‚Ç‚¿‚ç‘¤‚©‚ç‚ß‚è‚ñ‚Å‚¢‚é‚©‚ğZo‚·‚é
		if (move.GetX() < 0) {
			// ¶‚©‚çÚG‚µ‚Ä‚¢‚é
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		}
		else if (0 < move.GetX()) {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;
	}
}
