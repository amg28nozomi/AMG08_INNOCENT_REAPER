#include "SoulSkin.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "Game.h"
#include "ObjectServer.h"

namespace {
	constexpr auto SPEED = 7.5;
	constexpr auto SF_FRAME = 10;

	constexpr auto SOUL_I = 60;
	constexpr auto SOUL_IH = 30;
}

namespace inr {

	namespace soul {

	}

	SoulSkin::SoulSkin(Game& game) : ObjectBase(game), _moveVector() {
		_sType = Type::RED;
		Init();
	}

	SoulSkin::~SoulSkin() {

	}

	void SoulSkin::Init() {
		_motionKey = {
			{soul::B_FLOAT, { soul::SF_B_SOUL * SF_FRAME , SE_NUM}},
			{soul::R_FLOAT, { soul::SF_R_SOUL * SF_FRAME, SE_NUM}},
		};
		_divKey = { soul::R_FLOAT, key::SOUND_NUM };
		_position = { 500, 1000 };
		_speed = SPEED;
		_active = false;
	}

	void SoulSkin::Process() {
		if (_active) {
			_moveVector = { 0, 0 };
			AnimationCount();
			Tracking();
			Move();
		}
	}

	void SoulSkin::Draw() {
		if (_active) {
			Vector2 xy = _position;
			_game.GetMapChips()->Clamp(xy);
			auto x = xy.IntX();
			auto y = xy.IntY();

			int graph;	// グラフィックハンドル格納用
			GraphResearch(&graph);	// ハンドル取得
			DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
		}
	}

	void SoulSkin::Tracking() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();

		double px;
		if (player->GetDirection() == PL_LEFT) {
			switch (_sType) {
			case Type::BLUE:
				px = player->GetPosition().GetX() + SOUL_I + SOUL_I;
				break;
			case Type::RED:
				px = player->GetPosition().GetX() + SOUL_I;
				break;
			}
		} else {
			switch (_sType) {
			case Type::BLUE:
				px = player->GetPosition().GetX() - SOUL_I - SOUL_I;
				break;
			case Type::RED:
				px = player->GetPosition().GetX() - SOUL_I;
				break;
			}
		}
		auto py = player->GetPosition().GetY() - SOUL_IH;

		// 自身とプレイヤー間のベクトルを算出
		Vector2 mv = { px - _position.GetX(), py - _position.GetY() };
		mv.Normalize();

		if (0 <= mv.GetX() && mv.GetX() <= 0.1 || -0.1 <= mv.GetX() && mv.GetX() <= 0) mv.GetPX() = 0;
		if (0 <= mv.GetY() && mv.GetY() <= 0.1 || -0.1 <= mv.GetY() && mv.GetY() <= 0) mv.GetPY() = 0;

		// 移動ベクトルに加算
		_moveVector.GetPX() = mv.GetX() * _speed;
		_moveVector.GetPY() = mv.GetY() * _speed;
	}

	void SoulSkin::Move() {
		_position =  _position + _moveVector;	// 座標更新
	}


	void SoulSkin::SetStatus(Vector2 spawn, std::string soulcolor) {
		_position = spawn;
		if (soulcolor == "red") {
			_sType = Type::RED;
			_divKey = { soul::R_FLOAT, key::SOUND_NUM };
		}
		else if (soulcolor == "blue") {
			_sType = Type::BLUE;
			_divKey = { soul::B_FLOAT, key::SOUND_NUM };
		}
	}

	void SoulSkin::SetParameter(int soulcolor, double speed) {
		_speed = speed;
		switch (soulcolor) {
		case 1:
			_sType = Type::RED;
			_divKey = { soul::R_FLOAT, key::SOUND_NUM };
			break;
		case 2:
			_sType = Type::BLUE;
			_divKey = { soul::B_FLOAT, key::SOUND_NUM };
			break;
		}
	}

	void SoulSkin::SetSpwan(Vector2 spwan) {
		_position = spwan;
		_active = true;
	}

	bool SoulSkin::SoulColor() { 
		bool scolor;
		(_sType == Type::RED) ? scolor = soul::RED : scolor = soul::BLUE;
		return scolor;
	}
}
