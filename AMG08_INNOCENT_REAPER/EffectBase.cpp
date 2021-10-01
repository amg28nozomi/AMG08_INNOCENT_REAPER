#include "EffectBase.h"
#include "EffectBase.h"
#include "MapChips.h"
#include "ResourceServer.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include <DxLib.h>

namespace inr {

	EffectBase::EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, bool direction) : _game(game), _position(spawnpos),_collision(Vector2(), Vector2()) {
		_count = 0;
		_graphKey = gh;
		_alive = maxFrame;
		_delete = false;
		_direction = direction;	
		_isDamage = false;
		_loop = 0;
	}


	void EffectBase::Init() {

	}

	void EffectBase::Process() {
		++_count;
		if (_isDamage == true) {
			if(IsDamage() == true) Damage();
		}
		if (_count == (_alive - 1)) {
			if (_loop == 0) {
				_delete = true;	// 消去フラグをオンにする（消去予約）
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
				return;
			}
			--_loop;
			_count = 0;
		}
	}

	void EffectBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// 画像位置を修正する
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE, _direction);
		// デバッグ時のみ、当たり判定を描画する
#ifdef _DEBUG
		auto db = _collision;
		auto min = db.GetMin();
		auto max = db.GetMax();
		_game.GetMapChips()->Clamp(min);	// 画像位置を修正する
		_game.GetMapChips()->Clamp(max);	// 画像位置を修正する
		DxLib::DrawBox(min.IntX(), min.IntY(), max.IntX(), max.IntY(), GetColor(255, 255, 255), FALSE);
#endif
	}

	int EffectBase::GraphNumber() {
		auto allnum = graph::ResourceServer::GetAllNum(_graphKey);
		auto interval = _alive / allnum;	// 猶予時間の割り出し
		auto no = _count / interval % allnum;	// 描画する画像の算出
		return no;
	}

	void EffectBase::GraphResearch(int* gh) {
		*gh = graph::ResourceServer::GetHandles(_graphKey, GraphNumber());
	}

	void EffectBase::SetDamageEffect(int width, int height, int dinter) {
		// ダメージ判定を持たせる
		_isDamage = true;	// ダメージオン
		_collision = { _position, width / 2, height / 2, true };
		_dInter = dinter;
	}

	void EffectBase::SetDamageEffect(int width1, int width2, int height1, int height2, int dinter) {
		_isDamage = true;	// ダメージオン
		_collision = { _position, width1, width2, height1, height2, true };
		_dInter = dinter;
	}

	void EffectBase::SetLoop(int max) {
		if (max < 0) max = 0;
		_loop = max;
	}

	void EffectBase::Damage() {
		// ダメージ処理を行う
		auto player = _game.GetObjectServer()->GetPlayer();
		if (_collision.HitCheck(player->GetMainCollision()) != true) return;
		player->Damage(IsPlayerPosition());	// 自機のダメージ処理を呼び出す
	}

	bool EffectBase::IsPlayerPosition() {
		auto player = _game.GetObjectServer()->GetPlayer();
		auto pos = _position.GetX() - player->GetPosition().GetX();
		if (pos < 0) return false;
		else return true;
	}

	bool EffectBase::IsDamage() {
		bool isdamage = _dInter <= GraphNumber();
		return isdamage;
	}
}