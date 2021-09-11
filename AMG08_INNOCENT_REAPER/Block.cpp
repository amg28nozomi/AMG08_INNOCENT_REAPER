#include "Block.h"
#include "Game.h"

namespace inr {

	Block::Block(Game& game) : GimmickBase(game) {
		_gType = GimmickType::BLOCK;
		_divKey = { gimmick::block::KEY_BLOCK, "" };
		_motionKey.clear();

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;	// ���Ă��Ȃ�
	}

	void Block::Init() {

	}

	void Block::Process() {
		if (_break == gimmick::block::BRAKE_ON && _pal == 0) return;	// �������I������
		if (_break != gimmick::block::BRAKE_ON) return;	// ���Ă��Ȃ��ꍇ�������𔲂���
		_pal -= 5;	// ���Z�������s��
		if (_pal < 0) _pal = 0;	// �������Ȃ����ꍇ�͒l���C��
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
		// �Ώۂ͐ڐG���Ă��邩�H
		if (_mainCollision.HitCheck(box) == false) return false;	// �Փ˂��Ă��Ȃ�

		// �Փ˂��Ă���ꍇ�͂ǂ��瑤����߂荞��ł��邩���Z�o����
		if (move.GetX() < 0) {
			// ������ڐG���Ă���
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
