#include "TutorialImage.h"
#include "Game.h"
#include "Mapchips.h"
#include "ObjectServer.h"
#include "Player.h"
#include "TutorialValue.h"

namespace inr {

	TutorialImage::TutorialImage(Game& game) : Particle_Image(game), _collision(Vector2(), Vector2()) {
		_isCol = false;
	}

	/*void TutorialImage::Init() {

	}*/

	void TutorialImage::Process() {
		if (_isCol == true) IsCollision();
		// “–‚½‚è”»’è‚Í‚ ‚é‚©H
		switch (_animation) {
		case animation::A_ADD:
			if(AddPal() != true) return;
			break;
		case animation::A_SUB:
			if (SubPal() != true) return;
			_isDraw = false;	// •`‰æˆ—I—¹
			break;
		case animation::A_NORMAL:
			return;
		default:
			return;
		}
		_animation = animation::A_NORMAL;	// ’Êíó‘Ô‚É‚·‚é
	}

	void TutorialImage::Draw() {
#ifdef _DEBUG
		if (_game.IsDebugMode() == true) { 
			auto db = _collision;
			auto min = db.GetMin();
			auto max = db.GetMax();
			_game.GetMapChips()->Clamp(min);	// ‰æ‘œˆÊ’u‚ðC³‚·‚é
			_game.GetMapChips()->Clamp(max);	// ‰æ‘œˆÊ’u‚ðC³‚·‚é
			DxLib::DrawBox(min.IntX(), min.IntY(), max.IntX(), max.IntY(), GetColor(255, 255, 255), FALSE);
		}
#endif
		if (_isDraw != true) return;
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph = graph::ResourceServer::GetHandles(_graphKey);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_WIPE, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}

	void TutorialImage::IsCollision() {
		auto player = _game.GetObjectServer()->GetPlayer();
		// ÚG‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚Å
		if (_collision.HitCheck(player->GetMainCollision()) == true && _pal != 255) {
			if (_isDraw != true) _isDraw = true;
			if (_pal != 255) _animation = animation::A_ADD;
			return;
		} else if (_pal != 0) _animation = animation::A_SUB;
	}

	void TutorialImage::SetTutorialImage(TutorialValue tvalue) {
		auto ivalue = tvalue.GetImageValue();
		SetImage(ivalue);

		auto width = tvalue.Width();
		auto height = tvalue.Height();

		if (width.empty() == true ) {
			_isDraw = true;
			_isCol = false;
			return;
		}

		if (width.size() == 1 && (width[0] == 0 && height[0] == 0)) {
			_isDraw = true;
			_isCol = false;
			return;
		}
		_isCol = true;
		if (width.size() == 1) {
			_collision = AABB(_pos, width.at(0), height.at(0), true);
			return;
		}
		_collision = AABB(_pos, width.at(0), width.at(1), height.at(0), height.at(1), true);
	}

}
