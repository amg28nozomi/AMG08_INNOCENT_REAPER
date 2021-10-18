/*****************************************************************//**
 * @file   Crystal.cpp
 * @brief  �����N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Crystal.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "GimmickValue.h"
#include "SoundServer.h"

namespace inr {
	// �R���X�g���N�^
	Crystal::Crystal(Game& game) : GimmickBase(game) {
		_gType = GimmickType::CRYSTAL;	// �M�~�b�N�^�C�v�̐ݒ�
		_divKey = { "", gimmick::door::SE_CLOSE_DOOR };	// �摜�L�[�̐ݒ�
		// �e�평����
		_motionKey.clear();
		_soul.reset();
		_doors.clear();
	}
	// �`��
	void Crystal::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// ���W�̃N�����v
		// �`����W�̎Z�o
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		int graph = graph::ResourceServer::GetHandles(_divKey.first, 0);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		// �����蔻��̕`��
		DrawDebugBox(_mainCollision);
#endif
	}
	// �I�u�W�F�N�g���̓o�^
	void Crystal::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// �I�u�W�F�N�g���̓o�^
		_position = _oValue.Positions()[0];	// �����̍��W
		AddSoul();	// ���̐���
		GraphKey();	// �摜�L�[�̎Z�o
		// �h�A�̐���
		for (auto i = 0; i < _oValue.DoorType().size(); ++i) _doors.emplace_back(std::move(std::make_shared<Door>(_game.GetGame())));
		// �h�A�̒���
		std::vector<std::string> key;
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) {
			switch (_oValue.DoorType().at(i)) {
				// �Ԃ���
			case oscenario::gimmick::crystal::DOOR_RED:
				key.emplace_back(gimmick::door::KEY_DOOR_RED);
				break;
				// ����
			case oscenario::gimmick::crystal::DOOR_BLUE:
				key.emplace_back(gimmick::door::KEY_DOOR_BLUE);
				break;
			default:
				break;
			}
		}
		SetDoors(key);	// �h�A�̐ݒ�
		// �M�~�b�N�T�[�o�[�ɓo�^����
		for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) _game.GetGimmickServer()->Add(_doors.at(i));
		
		// �����蔻��
		_mainCollision = { _position, 50, 50, 30, 70, true };
	}
	// �h�A�̐ݒ�
	void Crystal::SetDoors(std::vector<std::string> key) {
		// ���̐F�����đΉ��������̂��J����
		if (_soul == nullptr) { // ������ł͂Ȃ���
			for (auto i = 0; i < static_cast<int>(_doors.size()); ++i) { 
				_doors[i]->SetParameter(_oValue.Positions()[1 + i], key.at(i), oscenario::gimmick::FLAG_FALSE); 
			}
			return;
		}
		if (_doors.size() == 1) {
			// �F���r
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
	// �`��摜�̐ݒ�
	void Crystal::GraphKey() {
		// �����̎�ނɉ����āA�L�[��؂�ւ���
		switch (_oValue.CrystalType()) {
		// ����̍Ւd(�X�e�[�W0�`�`���[�g���A���X�e�[�W)
		case oscenario::gimmick::crystal::TYPE_STAGE_0:
			// ���͋󂩁H
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_EMPTY;
				return;
			}
			// ���͉��F���H
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_RRD;
				return;
			case soul::BLUE:
				_divKey.first = gimmick::crystal::type1::KEY_CRYSTAL_BLUE;
				return;
			}
			// �_��̐X(�X�e�[�W2)
		case oscenario::gimmick::crystal::TYPE_STAGE_2:
			// ���͋󂩁H
			if (_soul == nullptr) {
				_divKey.first = gimmick::crystal::type2::KEY_CRYSTAL_EMPTY;
				return;
			}
			// ���͉��F���H
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
	// ���̐���
	void Crystal::AddSoul() {
		// ���͋󂩁H
		if (_oValue.SoulType() == 0) {
			_soul = nullptr;
			return;
		}
		// ���𐶐�
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());
		// ���̐������ɉ����āA�l��ύX
		switch (_oValue.SoulType()) {
		case 1:
			// ��
			soul->SetParameter(_oValue.SoulType(), 7);
			break;
		case 2:
			// ��
			soul->SetParameter(_oValue.SoulType(), 7);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error�FEnemyBase->SetParameter�@ObjectValue��_soulType�̒l���s���ł�\n");
#endif
			break;
		}
		// �e�퍰�̓o�^
		_soul = soul;
		_game.GetObjectServer()->Add(std::move(soul));
	}
	// ���@�A�N�V�����Ƃ̏Փ˔���
	void Crystal::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���͒D���邩�H
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_mainCollision.HitCheck(acollision)) {
					// ���𗼕�����
					for (auto door_c : _doors) door_c->SwitchOff();
					auto sound = se::SoundServer::GetSound(gimmick::crystal::KEY_ROB);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
					_soul->SetSpwan(_position);	// ���S���W�Ɏ��̉�������

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
						auto sound = se::SoundServer::GetSound(gimmick::crystal::KEY_GIVE);
						PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

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
	// �h�A�̉��
	bool Crystal::IsOpen(int type) {
		// �h�A�̐F�ƕۗL���Ă��鍰���r
		if (static_cast<int>(_soul->SoulColor()) != type) return false;
		// ��v���Ă���
		return true;
	}
	// �I�u�W�F�N�g���̍X�V
	void Crystal::ObjValueUpdate() {
		// ������̏ꍇ�͏�����Ԃɖ߂�
		if (_soul == nullptr) {
			_oValue.SoulUpdate(0);
			return;
		} else {
			// ���̏���o�^
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