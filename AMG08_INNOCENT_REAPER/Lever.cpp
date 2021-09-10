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
		_motionKey = { { _divKey.first, {20, 50}} };
	}

	void Lever::Init() {
	}

	void Lever::Process() {
		if (IsAnimationMax()) return;
		if (_door->IsSwitch()) {
			++_aCount;
		}
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
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
	}

	void Lever::SetParameter(ObjectValue objValue) {
		_oValue = objValue;

		_position = _oValue.Positions()[0];
		_mainCollision = {  _position, 30, 30, 50, 70, true  };	// 当たり判定の設定

		auto gdoor = std::make_shared<Door>(_game.GetGame());
		std::string gh;
		switch (_oValue.GimmickType()) {
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

		if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
			_aCount = AnimationCountMax();
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
		}

		gdoor->SetParameter(_oValue.Positions()[1], gh, _oValue.GimmickFlag());	// すぽーん
		_door = gdoor;
		_game.GetObjectServer()->Add(gdoor);
	}

	void Lever::ObjValueUpdate() {
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_door->IsSwitch() == gimmick::ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// 更新を行う
	}
}