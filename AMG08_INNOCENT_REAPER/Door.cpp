#include "Door.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
		_gType = GimmickType::DOOR;
		Init();
	}

	void Door::Init() {
		_switch = gimmick::OFF;
		_pal = 255;
	}

	void Door::Process() {
		if (_switch == gimmick::OFF) return;	// �t���O���I���̏ꍇ�͏������I������
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

	void Door::SetParameter(Vector2 spwan, std::string key) {
		_position = spwan;
		_mainCollision = { _position, 20, 20, 10, 70, true };
		_divKey.first = key;
		_motionKey = { { _divKey.first, {25, 50}} };
	}

	void Door::SwitchOn() {
		_switch = gimmick::ON;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}

	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// �Ώۂ͐ڐG���Ă��邩�H
		if (_mainCollision.HitCheck(box) == false) return false;	// �Փ˂��Ă��Ȃ�
		// �Փ˂��Ă���ꍇ�͂ǂ��瑤����߂荞��ł��邩���Z�o����
		if (move.GetX() < 0) {
			// ������ڐG���Ă���
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMin() + box.GetWidthMin();
			move.GetPX() = 0;
		} else {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;
	}
}