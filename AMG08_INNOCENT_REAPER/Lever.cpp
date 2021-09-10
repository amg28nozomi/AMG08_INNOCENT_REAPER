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
		if (_door->IsSwitch() == gimmick::ON) return;	// �h�A���I�[�v���̏ꍇ�͏����𔲂���
		auto sh = SoundResearch(gimmick::lever::KEY_LEVER);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
		_door->SwitchOn();	// �X�C�b�`�I��
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
	}

	void Lever::SetParameter(ObjectValue objValue) {
		_oValue = objValue;

		_position = _oValue.Positions()[0];
		_mainCollision = {  _position, 30, 30, 50, 70, true  };	// �����蔻��̐ݒ�

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
		gdoor->SetParameter(_oValue.Positions()[1], gh);	// ���ہ[��
		_door = gdoor;
		_game.GetObjectServer()->Add(gdoor);
	}

	void Lever::ObjValueUpdate() {
		_oValue.FlagUpdate(_door->IsSwitch());	// �X�V���s��
	}
}