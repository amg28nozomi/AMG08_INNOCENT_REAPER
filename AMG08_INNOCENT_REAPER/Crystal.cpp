#include "Crystal.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"

namespace inr {

	Crystal::Crystal(Game& game) : GimmickBase(game) {
		_gType = GimmickType::CRYSTAL;
		_divKey = { "", "" };
		_motionKey.clear();
		_soul.reset();
		_doors.clear();
	}

	void Crystal::Init() {

	}

	void Crystal::Process() {

	}

	void Crystal::Draw() {

	}

	void Crystal::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];	// 水晶の座標

		for (auto i = 0; i < _oValue.DoorType().size(); ++i) _doors.emplace_back(std::move(std::make_shared<Door>(_game.GetGame())));
		// ドアの調整
		std::string key = "";
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) {
			switch (_oValue.DoorType().at(i)) {
			case oscenario::gimmick::crystal::DOOR_RED:
				key = gimmick::door::KEY_DOOR_RED;
				break;
			case oscenario::gimmick::crystal::DOOR_BLUE:
				key = gimmick::door::KEY_DOOR_BLUE;
				break;
			default:
				break;	// そんなものはない
			}
		}

		
		_mainCollision = { _position, 50, 50, 30, 70, true };	// 当たり判定
	}

	void Crystal::SetDoors(std::string key) {
		// 魂の色を見て対応したものを開ける
		if (_soul == nullptr) { // 魂が空ではない時
			for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _doors[i].SetParameter(_oValue.Positions()[1 + i], key, gimmick::OFF);
			return;
		}
		if (_doors.size() == 1) {
			_doors[0].SetParameter(_oValue.Positions()[1], key, gimmick::ON);
			return;
		}
		// ドアが1つ以上ある場合
		std::vector<bool> flag;
		switch (_soul->SoulColor()) {
		case soul::RED:
			flag = { gimmick::ON, gimmick::OFF };
			break;
		case soul::BLUE:
			flag = { gimmick::ON, gimmick::OFF };
			break;
		}
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _doors[i].SetParameter(_oValue.Positions()[1 + i], key, flag.at(i));
	}
}