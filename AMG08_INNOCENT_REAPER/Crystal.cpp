#include "Crystal.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"
#include "ObjectServer.h"
#include "SoundServer.h"

namespace inr {

	Crystal::Crystal(Game& game) : GimmickBase(game) {
		_gType = GimmickType::CRYSTAL;
		_divKey = { "", gimmick::door::SE_CLOSE_DOOR };
		_motionKey.clear();
		_soul.reset();
		_doors.clear();
	}

	void Crystal::Init() {

	}

	void Crystal::Process() {
	}

	void Crystal::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph = graph::ResourceServer::GetHandles(_divKey.first, 0);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

	void Crystal::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];	// �����̍��W
		AddSoul();
		GraphKey();

		for (auto i = 0; i < _oValue.DoorType().size(); ++i) _doors.emplace_back(std::move(std::make_shared<Door>(_game.GetGame())));
		// �h�A�̒���
		std::vector<std::string> key;
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) {
			switch (_oValue.DoorType().at(i)) {
			case oscenario::gimmick::crystal::DOOR_RED:
				key.emplace_back(gimmick::door::KEY_DOOR_RED);
				break;
			case oscenario::gimmick::crystal::DOOR_BLUE:
				key.emplace_back(gimmick::door::KEY_DOOR_BLUE);
				break;
			default:
				break;	// ����Ȃ��̂͂Ȃ�
			}
		}
		SetDoors(key);
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _game.GetObjectServer()->Add(_doors.at(i));
		
		_mainCollision = { _position, 50, 50, 30, 70, true };	// �����蔻��
	}

	void Crystal::SetDoors(std::vector<std::string> key) {
		// ���̐F�����đΉ��������̂��J����
		if (_soul == nullptr) { // ������ł͂Ȃ���
			for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) { 
				_doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), oscenario::gimmick::FLAG_FALSE); 
			}
			return;
		}
		if (_doors.size() == 1) {
			// �F���r����i�A�C�e���j
			auto flag = oscenario::gimmick::FLAG_FALSE;
			if (_doors[0]->DoorColor() == static_cast<int>(_soul->SoulColor())) flag = oscenario::gimmick::FLAG_TRUE;
			_doors[0]->SetParameter(_oValue.Positions()[1], key.at(0), gimmick::ON);
			return;
		}
		// �h�A��1�ȏ゠��ꍇ
		std::vector<int> flag;
		switch (_soul->SoulColor()) {
		case soul::RED:
			flag = { oscenario::gimmick::FLAG_TRUE, oscenario::gimmick::FLAG_FALSE };
			break;
		case soul::BLUE:
			flag = { oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::FLAG_TRUE };
			break;
		}
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), flag.at(i));
	}

	void Crystal::GraphKey() {
		// �����̌����ڂ͂ǂ��炩
		switch (_oValue.CrystalType()) {
		case oscenario::gimmick::crystal::TYPE_STAGE_0:
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_EMPTY;
				return;
			}
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_RRD;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_BLUE;
				return;
			}
		case oscenario::gimmick::crystal::TYPE_STAGE_2:
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_EMPTY;
				return;
			}
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_RED;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type2::KEY_CRTSTAL_BLUE;
				return;
			}
			return;
		default:
			break;
		}
	}

	void Crystal::AddSoul() {
		if (_oValue.SoulType() == 0) {
			_soul = nullptr;
			return;
		}
		auto soul_c = std::make_shared<SoulSkin>(_game.GetGame());
		
		switch (_oValue.SoulType()) {
		case 1:
			soul_c->SetParameter(_oValue.SoulType(), 7);
			break;
		case 2:
			soul_c->SetParameter(_oValue.SoulType(), 7);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error�FEnemyBase->SetParameter�@ObjectValue��_soulType�̒l���s���ł�\n");
#endif
			break;
		}
		_soul = soul_c;
		_game.GetObjectServer()->Add(std::move(soul_c));
	}

	void Crystal::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���͒D���邩�H
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_mainCollision.HitCheck(acollision)) {
					auto sound = se::SoundServer::GetSound(gimmick::door::SE_CLOSE_DOOR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
					// ���𗼕�����
					for (auto door_c : _doors) door_c->SwitchOff();
					

					_soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������

					// ���@���ۗL���鍰����������ɓ��B���Ă���ꍇ�͏��L���������
					if (player->IsSoulMax()) {
						_soul->OwnerNull();
						_soul.reset();	// ���L���������
						GraphKey();	// �摜�؂�ւ�
						return;
					}
					player->SoulCatch(std::move(_soul));	// ���̏��L�����v���C���[�ɏ��n
					GraphKey();	// �摜�؂�ւ�
					return;
				}
			}
		}
		// ����^�����邩�H
		if (ckey == PKEY_GIVE) {
			// �v���C���[�͍����������Ă��邩�H
			if (player->HaveSoul()) {
				// ������̏ꍇ�Ƀ{�b�N�X���ڐG������
				if (_soul == nullptr) {
					// �ڐG���̔����AABB�ōs���i�D���A�N�V�����Ƃ͈Ⴂ�A�����ɂ�鐧���Ȃ��j
					if (_mainCollision.HitCheck(acollision)) {
						_soul = player->GiveSoul();	// �v���C���\����Ώۂ̍����󂯎��
						_soul->Inactive();	// ����񊈐���Ԃɂ���
						GraphKey();	// �摜�؂�ւ�
						// �Ή������h�A���J��
						for (auto door : _doors) {
							if (IsOpen(door->DoorColor()) != true) continue;
							door->SwitchOn();	// ��v�����ꍇ�͔����J��
							
						}
					}
				}
			}
		}
	}

	bool Crystal::IsOpen(int type) {
		if (static_cast<int>(_soul->SoulColor()) != type) return false;
		return true;
		// ���̐F�ƍ��̐F����v���Ă��邩�H
		/*auto color = 0;
		if (_soul->SoulColor() == soul::BLUE) color = 1;
		if (color != type) return false;
		return true;*/

	}


	void Crystal::ObjValueUpdate() {
		// ������̏ꍇ�͏�����Ԃɖ߂�
		if (_soul == nullptr) {
			// 
			_oValue.SoulUpdate(0);
			return;
		}
		else {
			// ����������
			switch (_soul->SoulColor()) {
			case soul::RED:
				_oValue.SoulUpdate(1);
				break;
			case soul::BLUE:
				_oValue.SoulUpdate(2);
				break;
			}
		}
	}
}