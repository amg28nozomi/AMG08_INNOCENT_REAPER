/*****************************************************************//**
 * @file   Block.cpp
 * @brief  壊れる岩クラス（ギミックベースクラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Block.h"
#include "Game.h"
#include "SoundServer.h"
#include "GimmickServer.h"

namespace inr {
	// コンストラクタ
	Block::Block(Game& game) : GimmickBase(game) {
		// 各種初期化
		_gType = GimmickType::BLOCK;
		_divKey = { gimmick::block::KEY_BLOCK, gimmick::block::KEY_BLOCK };	
		_motionKey = { 
			{gimmick::block::KEY_BLOCK, {1, 0}},
			{gimmick::block::KEY_BREAK, { 26 * 2, 0}} };

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;
	}
	// 更新
	void Block::Process() {
		// フラグがオンかつ、アニメーションが終了している場合は該当オブジェクトを消去する
		if (_break == gimmick::block::BRAKE_ON && AnimationCountMax() == true) {
			// 消去フラグをオンにする
			_delete = true;
			_game.GetGimmickServer()->DelOn();
			return;
		}
		if (_break != gimmick::block::BRAKE_ON) return;	// 壊れていない場合も処理を抜ける
		++_aCount;
	}
	// 描画
	void Block::Draw() {
		// 描画座標の算出
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// グラフィックハンドルの習得
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}
	// オブジェクト情報の登録
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
		// 破壊無し
		_break = gimmick::block::BRAKE_OFF;
		_pal = 255;
	}
	// オブジェクト情報の更新
	void Block::ObjValueUpdate() {
		// 破壊されているかどうかでフラグを切り替える
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// フラグ更新
	}
	// 押し出し処理
	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction) {
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
		} else if (0 < move.GetX()) {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;	// 衝突している
	}
	// 破壊処理
	void Block::Break() {
		auto sound = SoundResearch(_divKey.second);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		_break = gimmick::block::BRAKE_ON;			// 破壊フラグをオンにする
		_divKey.first = gimmick::block::KEY_BREAK;	// キーを破壊エフェクトに切り替え
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
	}
}
