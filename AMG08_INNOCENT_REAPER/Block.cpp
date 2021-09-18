#include "Block.h"
#include "Game.h"
#include "SoundServer.h"

namespace inr {

	Block::Block(Game& game) : GimmickBase(game) {
		_gType = GimmickType::BLOCK;
		_divKey = { gimmick::block::KEY_BLOCK, gimmick::block::KEY_BLOCK };
		_motionKey.clear();

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;	// 壊れていない
	}

	void Block::Init() {

	}

	void Block::Process() {
		if (_break == gimmick::block::BRAKE_ON && _pal == 0) return;	// 処理を終了する
		if (_break != gimmick::block::BRAKE_ON) return;	// 壊れていない場合も処理を抜ける
		_pal -= 5;	// 減算処理を行う
		if (_pal < 0) _pal = 0;	// 小さくなった場合は値を修正
	}

	void Block::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		for (auto fix = 0; fix < 3; ++fix) DrawRotaGraph(x, (y - (fix * 80)), 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

	void Block::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];
		_mainCollision = { _position, 45, 45, 170, 70, true };

		if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
			_break = gimmick::block::BRAKE_ON;
			_pal = 0;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			return;
		}
		_break = gimmick::block::BRAKE_OFF;
		_pal = 255;
	}

	void Block::ObjValueUpdate() {
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);
	}


	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
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
		else if (0 < move.GetX()) {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;
	}

	bool Block::Break() {
		auto sound = SoundResearch(_divKey.second);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		_break = gimmick::block::BRAKE_ON;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
		return true;
	}
}
