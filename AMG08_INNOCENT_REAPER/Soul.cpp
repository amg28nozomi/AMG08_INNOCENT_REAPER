#include "Soul.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "game.h"
#include "ObjectServer.h"

namespace {
	constexpr auto SPEED = 7.5;
	constexpr auto SF_FRAME = 10;

	constexpr auto SOUL_I = 50;
	constexpr auto SOUL_IH = 20;
}

namespace inr {

	namespace soul {

	}

	Soul::Soul(Game& game) : ObjectBase(game), _moveVector() {
		_sType = Type::RED;
		Init();
	}

	Soul::~Soul() {

	}

	void Soul::Init() {
		_motionKey = {
			{soul::B_FLOAT, { soul::SF_B_SOUL * SF_FRAME , SE_NUM}},
			{soul::R_FLOAT, { soul::SF_R_SOUL * SF_FRAME, SE_NUM}},
		};
		_divKey = { soul::R_FLOAT, key::SOUND_NUM };
		_position = { 500, 1000 };
	}

	void Soul::Process() {
		_moveVector = { 0, 0 };
		// アニメーションが終わっていない場合はカウントを増やす
		if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		else _aCount = 0;	// カウンター初期化
		Tracking();
		Move();

	}

	void Soul::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// グラフィックハンドル格納用
		GraphResearch(&graph);	// ハンドル取得
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		DrawFormatString(1800, 75, GetColor(255, 0, 0), "soul.x = %d", x);
		DrawFormatString(1800, 100, GetColor(255, 0, 0), "soul.y = %d", y);
	}

	void Soul::Tracking() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();

		double px;
		if (player.GetDirection() == PL_LEFT) {
			px = player.GetPosition().GetX() + SOUL_I;
		} else {
			px = player.GetPosition().GetX() - SOUL_I;
		}
		auto py = player.GetPosition().GetY() - SOUL_IH;

		// 自身とプレイヤー間のベクトルを算出
		Vector2 mv = { px - _position.GetX(), py - _position.GetY() };
		mv.Normalize();
		// 移動ベクトルに加算
		_moveVector.GetPX() = mv.GetX() * SPEED;
		_moveVector.GetPY() = mv.GetY() * SPEED;
	}

	void Soul::Move() {
		_position =  _position + _moveVector;	// 座標更新
	}
}
