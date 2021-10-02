#include "TutorialImage.h"
#include "Game.h"
#include "Mapchips.h"
#include "ObjectServer.h"
#include "Player.h"
#include "ImageValue.h"

namespace inr {

	TutorialImage::TutorialImage(Game& game) : Particle_Image(game), _collision(Vector2(), Vector2()) {

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
		if (_isDraw != true) return;
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph = graph::ResourceServer::GetHandles(_graphKey);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
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

		if (width.empty() == true) {
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
