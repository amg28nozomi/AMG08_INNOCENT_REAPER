#include "Lever.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Door.h"

namespace inr {

	Lever::Lever(Game& game) : GimmickBase(game) {
		_gType = GimmickType::LEVER;
		_door.reset();
		_divKey = { gimmick::lever::KEY_LEVER, "" };
		_motionKey = { { _divKey.first, {5, 50}} };
	}

	void Lever::Init() {
	}

	void Lever::Process() {

	}

	void Lever::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);

		DrawDebugBox(_mainCollision);
	}

	void Lever::OpenDoor() {
		if (_door->IsSwitch() == gimmick::ON) return;	// ドアがオープンの場合は処理を抜ける
		auto sh = SoundResearch(gimmick::lever::KEY_LEVER);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
		_door->SwitchOn();	// スイッチオン
	}

	void Lever::SetParameter(Vector2 spawnL, Vector2 spawnD, int doorno) {
		_position = spawnL;
		_mainCollision = {  _position, 30, 30, 50, 70, true  };	// 当たり判定の設定

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
		gdoor->SetParameter(spawnD, gh);	// すぽーん
		_door = gdoor;
		_game.GetObjectServer()->Add(gdoor);
	}
}