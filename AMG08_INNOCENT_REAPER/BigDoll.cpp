#include "BigDoll.h"
#include "SoundServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoulSkin.h"
#include <memory>

namespace {
	constexpr auto BIG_EMPTY_WIDTH = 200;
	constexpr auto BIG_EMPTY_HEIGHT = 100;

	constexpr auto PATROL_MAX = 280;
	constexpr auto PATROL_VECTOR = 40.0 / 60.0;

	constexpr auto BIG_DOLL_VITAL = 40;

	constexpr auto STAY_MAX = 60;

	// �U��
	constexpr auto ATTACK_VECTOR_MIN = 1.0;
	constexpr auto ATTACK_VECTOR_MAX = 2.5;
	constexpr auto ADD_VECTOR = 0.1;

	constexpr auto TACKLE_MAX = 1200;
}

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::BIG_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::BIG_EMPTY, key::SOUND_NUM);
		_position = { 500, 500 };
		_atkVec = 0;

		Init();
	}

	BigDoll::~BigDoll() {

	}

	void BigDoll::Init() {
		// �����蔻��
		_mainCollision = { _position, enemy::BIG_WIDTH / 2, enemy::BIG_HEIGHT / 2, true };
		_collisions = {
			// �����k
			{enemy::BIG_EMPTY, {_position, BIG_EMPTY_WIDTH / 2, BIG_EMPTY_WIDTH / 2, 0, BIG_EMPTY_HEIGHT, true}},
		};
		_motionKey = {
			{ enemy::BIG_EMPTY, {40, 0}},
			
			{ enemy::red::BIG_WAKEUP, {20, 0}},
			{ enemy::red::BIG_IDOL, {15, 0}},
			{ enemy::red::BIG_PATROL, {20, 0}},
			{ enemy::red::BIG_TACKLE, {20, 0}},
			{ enemy::red::BIG_HIPDROP, {28, 0}},

			{ enemy::blue::BIG_WAKEUP, {20, 0}},
			{ enemy::blue::BIG_IDOL, {15, 0}},
			{ enemy::blue::BIG_PATROL, {20, 0}},
			{ enemy::blue::BIG_ESCAPE, {20, 0}},
		};
	}

	void BigDoll::Process() {
		ObjectBase::Process();	// �d�͏���
		_moveVector = { 0, 0 };	// ������

		AnimationCount();
		Action();
		StateUpdate();
		PositionUpdate();
	}


	void BigDoll::HipDrop() {

	}

	void BigDoll::StateUpdate() {
		if (_soul == nullptr) return;
		if (0 < _stay) {
			--_stay;
			return;
		}

		switch (_aState) {
		case ActionState::IDOL:
			PatrolOn();
			return;
		case ActionState::PATROL:
			// ���ړ�
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				if (0 < _patrolX) _patrolX = 0;
				return;
				// �E�ړ�
			}
			else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				if (_patrolX < 0) _patrolX = 0;
				return;
			}
			// �ړ��ʂ�0�̏ꍇ�͑ҋ@��ԂɑJ�ڂ���
			_aState = ActionState::IDOL;
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = enemy::red::BIG_IDOL;
				break;
			case soul::BLUE:
				_divKey.first = enemy::blue::BIG_IDOL;
				break;
			}
			_stay = STAY_MAX;
			_searchBox.GetCollisionFlgB() = false;	// ���G��؂�
			return;
		case ActionState::ATTACK:
			// ���ړ�
			if (_actionX < 0) {
				if(-ATTACK_VECTOR_MAX < _atkVec ) _atkVec += ADD_VECTOR;
				_actionX += _atkVec;
				_moveVector.GetPX() = -_atkVec;
				if (0 < _actionX) _actionX = 0;

			} else if (0 < _actionX) {
				if (_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX -= _atkVec;
				_moveVector.GetPX() = _atkVec;
				if (_actionX < 0) _actionX = 0;
			}
			if(_actionX == 0) ChangeIdol();
			return;
		case ActionState::ESCAPE:
			// ���ړ�
			if (_actionX < 0) {
				_actionX += (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = -(enemy::ESCAPE_VECTOR / 30);
				if (0 < _actionX) _actionX = 0;

			}
			else if (0 < _actionX) {
				_actionX -= (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = (enemy::ESCAPE_VECTOR / 30);
				if (_actionX < 0) _actionX = 0;
			}
			if (_actionX == 0) ChangeIdol();
			return;
			// �N���オ�胂�[�V������
		case ActionState::WAKEUP:
			// �N���オ��A�j���[�V�����̍Đ����I������Ȃ�A�����ԂɑJ�ڂ���
			if (IsAnimationMax()) {
				ChangeIdol();
				_stay = STAY_MAX;
#ifdef _DEBUG
				_searchBox.GetbDrawFlg() = true;
#endif

			}
			return;
		}
	}

	void BigDoll::Move() {
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}

	void BigDoll::Death() {
		ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
		EnemyBase::Death();
	}

	void BigDoll::SetParameter(ObjectValue objValue) {
		EnemyBase::SetParameter(objValue);
		if (_oValue.SoulType() == 0) {	// ������̏ꍇ�͔����k�ɂȂ�
			ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
			_aCount = AnimationCountMax();	// �J�E���^���}�b�N�X�ɂ���
			_changeGraph = true;
			return;	// �����𔲂���
		}
		auto soul_n = std::make_shared<SoulSkin>(_game.GetGame());
		// auto sn = std::static_pointer_cast<SoulSkin>(soul_n);
		switch (_oValue.SoulType()) {
		case 1:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::red::BIG_PATROL);
			break;
		case 2:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::blue::BIG_PATROL);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error�FEnemyBase->SetParameter�@ObjectValue��_soulType�̒l���s���ł�\n");
#endif
			break;
		}
		_soul = soul_n;
		_game.GetObjectServer()->Add(std::move(soul_n));
	}

	void BigDoll::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		_moveVector.GetPY() = _gravity;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction);
		GimmickCheck(_moveVector);
		_position = _position + _moveVector;	// �ʒu���W���X�V

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		auto col = _collisions.find(enemy::BIG_EMPTY);
		col->second.Update(_position, _direction);

		if (_soul == nullptr && IsAnimationMax() == true) {
			_mainCollision.Swap(col->second);
			_searchBox.GetbDrawFlg() = false;

		}
	}

	void BigDoll::PatrolOn() {
		if (_soul == nullptr) return;
		EnemyBase::PatrolOn();

		// ���̐F�ɉ����ăL�[��؂�ւ�
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::BIG_PATROL : _divKey.first = enemy::red::BIG_PATROL;

		if (_actionX == 0) {
			if (_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}

	void BigDoll::AttackOn() {
		if (_aState != ActionState::ATTACK) {
			ChangeState(ActionState::ATTACK, enemy::red::BIG_TACKLE);
			_searchBox.GetCollisionFlgB() = true;
			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX = -TACKLE_MAX;
				return;
			case enemy::MOVE_RIGHT:
				_actionX = TACKLE_MAX;
				return;
			}
		}
	}

	AABB BigDoll::NowCollision(std::string key) {
		if (_soul != nullptr) return _mainCollision;
		auto it = _collisions.find(enemy::BIG_EMPTY);
		// ���݂̃A�N�V������Ԃ̓{�b�N�X���C������K�v�����邩�H
		return it->second;
	}

	void BigDoll::EscapeOn() {
		if (_aState != ActionState::ESCAPE) {
			ChangeState(ActionState::ATTACK, enemy::blue::BIG_ESCAPE);
			_searchBox.GetCollisionFlgB() = true;
			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX = -enemy::ESCAPE_MAX;
				return;
			case enemy::MOVE_RIGHT:
				_actionX = enemy::ESCAPE_MAX;
				return;
			}
		}
	}

	void BigDoll::ChangeIdol() {
		_searchBox.GetCollisionFlgB() = false;	// �����蔻���؂�
		std::string nextkey = "";
		switch (_soul->SoulColor()) {
		case soul::RED:
			nextkey = enemy::red::BIG_IDOL;
			break;
		case soul::BLUE:
			nextkey = enemy::blue::BIG_IDOL;
			break;
		}
		ChangeState(ActionState::IDOL, nextkey);
	}

	void BigDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// ���݂̋}����������W���Z�o
		auto vitalPart = VitalPart(_mainCollision, BIG_DOLL_VITAL);
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���͒D���邩�H
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// ����D����
					ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
					_searchBox.GetCollisionFlgB() = false;	// �ꎞ�I�ɍ��G�����؂�

					_soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������

					// ���@���ۗL���鍰����������ɓ��B���Ă���ꍇ�͏��L���������
					if (player->IsSoulMax()) {
						_soul->OwnerNull();
						_soul.reset();	// ���L���������
						return;
					}
					player->SoulCatch(std::move(_soul));	// ���̏��L�����v���C���[�ɏ��n
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
						// �v���C���[���擾
						auto player = _game.GetObjectServer()->GetPlayer();
						_soul = player->GiveSoul();	// �v���C���\����Ώۂ̍����󂯎��
						_soul->Inactive();	// ����񊈐���Ԃɂ���
						switch (_soul->SoulColor()) {
						case soul::RED:
							ChangeState(ActionState::WAKEUP, enemy::red::BIG_WAKEUP);
							return;
						case soul::BLUE:
							ChangeState(ActionState::WAKEUP, enemy::blue::BIG_WAKEUP);
							return;
						}
						return;
					}
				}
			}
		}
	}
}