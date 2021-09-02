#include "Lever.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Door.h"

namespace inr {

	Lever::Lever(Game& game) : GimmickBase(game) {
		_door.reset();
		_door = std::make_shared<Door>(_game.GetGame());
	}

	void Lever::Init() {
	}

	void Lever::Process() {

	}

	void Lever::Draw() {

	}

	void Lever::SetParameter(Vector2 spawnL, Vector2 spawnD, int doorno) {
		_position = spawnL;
		_mainCollision = {  _position, 140, 140, true  };	// ìñÇΩÇËîªíËÇÃê›íË

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
		gdoor->SetParameter(spawnD, gh);	// Ç∑Ç€Å[ÇÒ
		_door = gdoor;
		_game.GetObjectServer()->Add(gdoor);
	}
}