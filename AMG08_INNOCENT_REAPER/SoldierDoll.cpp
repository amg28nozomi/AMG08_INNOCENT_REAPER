#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "Player.h"
#include "ResourceServer.h"

// ����
#include "SoulSkin.h"
#include <memory>

#ifdef _DEBUG
namespace {
	constexpr auto START_POS_X = 500;
	constexpr auto START_POS_Y = 0;

	// ���E����
	constexpr auto SOLDIER_W = 60;
	constexpr auto SOLDIER_H = 120;

	// �ʏ��Ԃ̓����蔻��(�C���p)
	constexpr auto SF_HEIGHT = 20;
	constexpr auto SOLDIER_BOX_W = SOLDIER_W / 2;
	constexpr auto SOLDIER_BOX_H1 = SOLDIER_H / 2 - SF_HEIGHT;
	constexpr auto SOLDIER_BOX_H2 = SOLDIER_H / 2 + SF_HEIGHT;

	// ���͈� 
	constexpr auto SOLDIER_VITAL = 20;
	// �����蔻��
	constexpr auto COLLISION_W = 60;

	constexpr auto EMPTY_W = 250;
	constexpr auto EMPTY_H = 100;

	// ���G�͈�
	constexpr auto SEARCH_X = 100 + (SOLDIER_BOX_W * 2);
	constexpr auto SEARCH_Y1 = SOLDIER_BOX_H1;
	constexpr auto SEARCH_Y2 = SOLDIER_BOX_H2;

	// ����͈�(�ő�Vector)
	constexpr auto PATROL_VECTOR = 1.0;
	constexpr auto PATROL_MAX = 280;

	constexpr auto FRAME = 30;
}
#endif
namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::IDOL;
		_sState = SoulState::BLUE;
		_divKey = std::make_pair(enemy::blue::SOLDIER_IDOL, key::SOUND_NUM);
		_position = { START_POS_X, START_POS_Y };

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_patrolX = 0;
		_aInterval = 0;
		_direction = true;
		_changeGraph = true;
		_drawStop = false;

		Init();
	}

	SoldierDoll::~SoldierDoll() {
	}

	void SoldierDoll::Init() {
		// ���C���̓����蔻���������
		_mainCollision = { _position, SOLDIER_BOX_W, SOLDIER_BOX_W, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_searchBox = { _position, SEARCH_X, SEARCH_X, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };

		/*_searchBox = { { _mainCollision.GetMin().GetX() - SEARCH_X, _mainCollision.GetMin().GetY() - SEARCH_Y }, {
						_mainCollision.GetMax().GetX() + SEARCH_X, _mainCollision.GetMax().GetY() + SEARCH_Y } };*/
		// �e�픻��̊i�[�i�U������E�j
		_collisions = {
			{ enemy::SOLDIER_EMPTY, { _position, 65, 60, -45, 80 , true}},	// �����k���̏��
		};

		_motionKey =
		{	{ enemy::SOLDIER_EMPTY, {40, 0}},

			{ enemy::red::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::red::SOLDIER_IDOL, {32, 0}},
			{ enemy::red::SOLDIER_PATROL, {36, 0}},
			{ enemy::red::SOLDIER_ATTACK, {12, 50}},	// SE�L��

			{ enemy::blue::SOLDIER_WAKEUP, {20, 0}},
			{ enemy::blue::SOLDIER_IDOL, {28, 0}},
			{ enemy::blue::SOLDIER_PATROL, {42, 0}},
			{ enemy::blue::SOLDIER_ESCAPE, {24, 0}}
		};

		PatrolOn();
	}

	void SoldierDoll::Process() {
		ObjectBase::Process();
		_moveVector.GetPX() = 0;

		// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
		AnimationCount();

		Patrol();
		Action();

		// �����蔻����擾

		Move();
		PositionUpdate();
	}

	void SoldierDoll::StateUpdate() {
		switch (_sState) {
		// �����Ȃ��ꍇ�͉������Ȃ���[
		case SoulState::EMPTY:
			// �E�o
			break;
		// �����̏ꍇ
		case SoulState::BLUE:
			// ����(���G)���������ҋ@
			// �C���^�[�o��������ꍇ��interval�����������ď����𔲂���
			if (_aInterval) { 
				--_aInterval; 
				break;
			}
			// ���G���s��

			break;
		// �Ԃ����̏ꍇ
		case SoulState::RED:
			if (_aInterval) {
				--_aInterval;
				break;
			}

			break;
		// ������̎��̏���
		/*case ActionState::EMPTY:
			break;
		case ActionState::IDOL:
			break;
		case ActionState::PATROL:
			break;
		case ActionState::ATTACK:
			break;
		case ActionState::ESCAPE:
			break;
		default:
			break;*/
		}
	}

	void SoldierDoll::Patrol() {
		// ������̏ꍇ�̓[���ɂ���
		if (_aState == ActionState::EMPTY) _patrolX = 0;
		else if (_aState != ActionState::PATROL && _aState != ActionState::ESCAPE && _aState!= ActionState::IDOL) {
			PatrolOn();
		}
		if (_aState == ActionState::PATROL) {
			// ���ړ�
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				_moving = enemy::MOVE_LEFT;
				return;
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				_moving = enemy::MOVE_RIGHT;
			}

			if (static_cast<int>(_patrolX) == 0) {
				if (_moving == enemy::MOVE_LEFT) _patrolX = PATROL_MAX;
				else _patrolX = -PATROL_MAX;
			}
		}
	}

	void SoldierDoll::Move() {
		// �ړ��x�N�g���ɉ����āA������ύX
		// �ړ��ʂɉ����Č�����ύX
		if (_moveVector.GetX() < 0) {
			_direction = false;
		}
		else if (0 < _moveVector.GetX()) {
			_direction = true;
		}

		// ���G�͈͂̈ړ�������280pixel
		if (_actionX != 0) {
			switch (_moving) {
			case enemy::MOVE_LEFT:
				_actionX -= enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
				break;
			case enemy::MOVE_RIGHT:
				_actionX += enemy::ESCAPE_VECTOR / FRAME;
				_moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
			}

		}
	}

	void SoldierDoll::Action() {
		// �v���C���[�𔭌��ł��邩
		if (SearchPlayer() == true) {
			// �����Ă��鍰�ɉ����ď�����ύX����
			switch (_sState) {
			// �Ԃ����̎��́A�ːi���������s����B
			case SoulState::RED:
				// Rush();
				break;
			case SoulState::BLUE:
				EscapeOn();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			default:
				break;
			}
		}
		// �����ł��Ȃ������ꍇ�͈ړ��������s��
		if (_sState == SoulState::EMPTY) _actionX = 0;
	}

	void SoldierDoll::PatrolOn() {
		_changeGraph = true;
		_aState = ActionState::PATROL;

		// ���̐F�ɉ����ăL�[��؂�ւ�
		(_sState == SoulState::BLUE) ? 
			_divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

		if (_actionX <= 0) {
			if(_direction) _patrolX = PATROL_MAX;
			else _patrolX = -PATROL_MAX;
		}
	}

	void SoldierDoll::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		_moveVector.GetPY() = _gravity;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		_game.GetMapChips()->IsHit(_mainCollision, _position,_moveVector, _direction);
		_position = _position + _moveVector;	// �ʒu���W���X�V

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		if (_sState == SoulState::EMPTY && IsAnimationMax() == true) {
			auto col = _collisions.find(enemy::SOLDIER_EMPTY);
			col->second.Update(_position, _direction);
			_mainCollision.Swap(col->second);
			_searchBox.GetbDrawFlg() = false;
			
		}
	}

	bool SoldierDoll::Hit() {
		// �v���C���[�̎Q�Ƃ��擾
		auto player = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);

		// �v���C���[�͎��g�ɐڐG���Ă��邩�ǂ����H
		if (_mainCollision.HitCheck(player.GetMainCollision())) {
			player.GetMainCollision();
		}
		return false;
	}

	void SoldierDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// ���݂̋}����������W���Z�o
		auto vitalPart = VitalPart(_mainCollision);
		// ���͒D���邩�H
		if (ckey == PKEY_ROB) {
			if (_sState != SoulState::EMPTY) {
				if (_direction != direction && vitalPart.HitCheck(acollision)) {
					// ����D����
					std::string scolor;
					double sp;
					if (_sState == SoulState::BLUE) {
						scolor = "blue";
						sp = 7;
					}
					else if (_sState == SoulState::RED) {
						scolor = "red";
						sp = 7.5;
					}

					_divKey.first = enemy::SOLDIER_EMPTY;
					_aState = ActionState::EMPTY;
					_sState = SoulState::EMPTY;
					_changeGraph = true;

					auto bSoul = std::make_unique<SoulSkin>(_game.GetGame());

					bSoul->SetStatus(_position, scolor);
					bSoul->SetSpeed(sp);
					_game.GetObjectServer()->Add(std::move(bSoul));
				}
			}
		}
	}

	AABB SoldierDoll::VitalPart(Collision& col) {
		// ���W���Z�o�iy���W�͕ύX�Ȃ��j
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		if (_direction) {
			// �E�Ɍ����Ă���ꍇ
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + SOLDIER_VITAL;
		} else {
			vitalMin.GetPX() = col.GetMax().GetX() - SOLDIER_VITAL;
			vitalMax.GetPX() = col.GetMax().GetX();
		}
		return AABB(vitalMin, vitalMax, true);
	}
}