#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "SoulSkin.h"

#include <random>
#include <time.h>
#include <memory>

namespace {
	constexpr auto CROW_WIDTH = 60;	// ���̓����蔻��
	constexpr auto CROW_HEIGHT = 180;	// �c�̓����蔻��

	constexpr auto CROW_VITAL = 20;
	constexpr auto CROW_LIFE_MAX = 10;
	constexpr auto CROW_ANGER = 5;	// �{���ԂɂȂ邩
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::CROW_DOLL;
		Init();
	}

	void CrowDoll::Init() {
		_target = { 0, 0 };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// �����蔻��
		_collisions = {
			{enemy::crowdoll::CROW_RASH, {_position, 0, 120, 130 ,CROW_HEIGHT / 2, true}},	// �A���U���̓����蔻��
			{enemy::}
		};

	}

	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;

	}

	void CrowDoll::Process() {

	}

	void CrowDoll::Draw() {

	}

	void CrowDoll::WakeUp() {
		// �N���オ�胂�[�V����

	}

	void CrowDoll::GetTarget() {
		_target = _game.GetObjectServer()->GetPlayer()->GetPosition();	// �ڕW�̌��ݒn�_���擾����
	}

	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;
		_cState = nextState;
		_changeGraph = true;
	}

	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_cState == CrowState::DEATH) return;	// ����ł���ꍇ�͍���D���Ȃ�
		if (IsVital() != true) return;	// �����Ȃ��ꍇ������D���Ȃ�
		if (ckey == PKEY_ROB) {	// ����D���ɂ��Ă��邩�H
			auto vitalPart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// ����D����
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// ���ݏ�Ԃɂ���
					AddSoul();	// ���𐶂ݏo��
					// �����Ŏ��S�������s����������s��
					return;
			}
		}
	}

	bool CrowDoll::IsVital() {
		switch (_cState) {
		case CrowState::IDOL:
		case CrowState::DEBUF:
		case CrowState::ROAR:
		case CrowState::WINCE:
		case CrowState::SLEEP:
				return false;
		case CrowState::RASH:
			return true;
		default:
			return false;
		}
	}

	void CrowDoll::AddSoul() {
		auto player = _game.GetObjectServer()->GetPlayer();	// ���@�̏����擾����
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());	// ���𐶐�����
		srand((unsigned int)time(NULL));	// ����������
		auto type = rand() % 2;
		switch (type) {
		case 0:
			soul->SetParameter(1, 7);	// �Ԃ̍�
			break;
		case 1:
			soul->SetParameter(2, 7);	// �̍�
			break;
		default:
			break;
		}
		soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������
		// ���@�͍��̏�������ɓ��B���Ă��邩�H
		if (player->IsSoulMax()) {
			_soul->OwnerNull();	// ���L�҂͂��Ȃ�
			_game.GetObjectServer()->Add(std::move(soul));	// �I�u�W�F�N�g�T�[�o�[�ɓo�^����
			return;
		}
		player->SoulCatch(std::move(_soul));	// �����ł͂Ȃ��ꍇ�͍��̏��L�����v���C���[�ɏ��n
	}

	bool CrowDoll::IsAnger() {
		if (CROW_ANGER < _life) return false;	// �{���ԂɂȂ��Ă��Ȃ�
		return true;
	}

	AABB CrowDoll::NowCollision(std::string key) {
		return _mainCollision;
	}
}
