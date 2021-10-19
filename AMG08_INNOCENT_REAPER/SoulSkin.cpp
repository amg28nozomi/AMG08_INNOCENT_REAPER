/*****************************************************************//**
 * @file   SoulSkin.cpp
 * @brief  魂クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
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
		_type = ObjectType::SOUL;
		_sType = Type::RED;
		_space = 0;
		_isOwner = false;
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
		_mainCollision = { _position, 50, 50, false};
		_speed = SPEED;
		_give = false;
		_active = false;
	}

	void SoulSkin::Process() {
		if (_active) {
			_moveVector = { 0, 0 };
			AnimationCount();
			Tracking();
			Move();
			Give();
		}
		if (_space != 0) _space = 0;
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
		if (_space == 0) return;
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();

		double px;
		if (player->GetDirection() == PL_LEFT) {
			px = player->GetPosition().GetX() + _space;
		} else {
			px = player->GetPosition().GetX() - _space;
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
		_mainCollision.Update(_position, _direction);
		if (_isOwner == false && _mainCollision.GetCollisionFlag() == false) _mainCollision.SetCollisionFlag() = true;
	}

	void SoulSkin::Give() {
		// 所有者がいる場合は飛ばす
		if (_isOwner == true) return;
		auto player = _game.GetObjectServer()->GetPlayer();
		if (player->IsSoulMax() == true) return;
		if(_mainCollision.HitCheck(player->GetMainCollision())) {
			_give = true;
			// 接触した場合は自身の所有権を付与
			player->SoulCatch(_game.GetObjectServer()->GetSoul());
			_give = false;
			_isOwner = true;
			_mainCollision.SetCollisionFlag() = false;
		}
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
		_isOwner = true;
		switch (soulcolor) {
		case 1:
			_sType = Type::RED;
			_divKey = { soul::R_FLOAT, key::SOUND_NUM };
			break;
		case 2:
			_sType = Type::BLUE;
			_divKey = { soul::B_FLOAT, key::SOUND_NUM };
			break;
		default:
			break;
		}
	}

	void SoulSkin::SetSpwan(Vector2 spawn) {
		_position = spawn;
		_active = true;
	}

	bool SoulSkin::SoulColor() { 
		bool scolor;
		(_sType == Type::RED) ? scolor = soul::RED : scolor = soul::BLUE;
		return scolor;
	}
}
