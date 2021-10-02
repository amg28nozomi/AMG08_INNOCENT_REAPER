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

	void TutorialImage::SetImage(ImageValue ivalue, int width, int height) {
		Particle_Image::SetImage(ivalue);
		if (width == 0 && height == 0) {
			_isCol = false;
			return;
		}
		_collision = AABB(_pos, width, height, true);
	}

	void TutorialImage::SetImage(ImageValue ivalue, int width1, int width2, int height1, int height2) {
		Particle_Image::SetImage(ivalue);
		_isCol = true;
		_collision = AABB(_pos, width1, width2, height1, height2, true);
	}

}
