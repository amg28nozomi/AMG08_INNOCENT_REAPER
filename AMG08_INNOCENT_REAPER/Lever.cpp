#include "Lever.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Door.h"

namespace inr {

	Lever::Lever(Game& game) : GimmickBase(game) {
		_door.reset();
	}

	void Lever::Init() {
		_door = std::make_shared<Door>(_game.GetGame());
	}

	void Lever::Process() {

	}

	void Lever::Draw() {

	}

	void Lever::SetParameter(Vector2 spawnL, Vector2 spawnD, int doorno) {
		_position = spawnL;
		auto gdoor = std::make_shared<Door>(_game.GetGame());
		std::string gh;
		switch (doorno) {
		case gimmick::door::D_LEVER:
			gh = gimmick::door::KEY_DOOR_LEVER;
			break;
		case gimmick::door::D_RED:
			gh = gimmick::door::D_RED;
			break;
		case gimmick::door::D_BLUE:
			gh = gimmick::door::D_BLUE;
			break;
		default:
			return;
		}
		gdoor->SetParameter(spawnD, gh);	// ВЈВџБ[Вс
		_door = gdoor;
		_game.GetObjectServer()->Add(gdoor);
	}
}