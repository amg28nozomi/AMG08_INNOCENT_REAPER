#include "Block.h"
#include "Game.h"
#include "SoundServer.h"
#include "GimmickServer.h"

namespace inr {

	Block::Block(Game& game) : GimmickBase(game) {
		_gType = GimmickType::BLOCK;	// ギミックタイプの設定
		_divKey = { gimmick::block::KEY_BLOCK, gimmick::block::KEY_BLOCK };	
		_motionKey = { 
			{gimmick::block::KEY_BLOCK, {1, 0}},
			{gimmick::block::KEY_BREAK, { 26 * 2, 0}} };

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;	// 壊れていない
	}

	void Block::Init() {

	}

	void Block::Process() {
		// フラグがオンかつ、アニメーションが終了している場合は該当オブジェクトを消去する
		if (_break == gimmick::block::BRAKE_ON && AnimationCountMax() == true) {
			_delete = true;
			_game.GetGimmickServer()->DelOn();
			return;
		}
		if (_break != gimmick::block::BRAKE_ON) return;	// 壊れていない場合も処理を抜ける
		++_aCount;
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
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

	void Block::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// オブジェクト情報登録
		_position = _oValue.Positions()[0];	// 座標の設定
		_mainCollision = { _position, 45, 45, 100, 140, true };	// 当たり判定の登録

		// フラグがオンの場合
		if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
			_break = gimmick::block::BRAKE_ON;	// 破壊済み
			_pal = 0;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			return;
		}
		_break = gimmick::block::BRAKE_OFF;
		_pal = 255;
	}

	void Block::ObjValueUpdate() {
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// フラグ更新
	}


	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		// 対象の判定座標更新
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// 対象は接触しているか？
		if (_mainCollision.HitCheck(box) == false) return false;	// 衝突していない

		// 衝突している場合はどちら側からめり込んでいるかを算出
		// 対応した方向へ押し出し処理を行う
		if (move.GetX() < 0) {
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
		_break = gimmick::block::BRAKE_ON;	// 破壊フラグをオンにする
		_divKey.first = gimmick::block::KEY_BREAK;	// キーを破壊エフェクトに切り替え
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
		return true;
	}
}
