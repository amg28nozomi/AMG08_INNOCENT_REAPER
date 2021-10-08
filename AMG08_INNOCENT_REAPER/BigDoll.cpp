/*****************************************************************//**
 * \file   BigDoll.cpp
 * \brief  �r�b�O�h�[���N���X�i�G�l�~�[�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include "BigDoll.h"
#include "SoundServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "SoulSkin.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "Block.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include <memory>

namespace {
	// �����k���̔���͈�
	constexpr auto BIG_EMPTY_WIDTH = 200;
	constexpr auto BIG_EMPTY_HEIGHT = 100;
	// ���񎞂̑��ړ���
	constexpr auto PATROL_MAX = 280;
	// ���񎞂̈ړ���
	constexpr auto PATROL_VECTOR = 40.0 / 60.0;
	// �}���̔���͈�
	constexpr auto BIG_DOLL_VITAL = 40;
	// �ҋ@�t���[��
	constexpr auto STAY_MAX = 60;
	// �ҋ@���[�V�������Đ�����
	constexpr auto BIG_STAY = 11 * 3;
	// �U�����̑��ړ���
	constexpr auto TACKLE_MAX = 1200;
	// �U�����̍Œ�ړ���
	constexpr auto ATTACK_VECTOR_MIN = 1.0;
	// �U�����̍ő�ړ���
	constexpr auto ATTACK_VECTOR_MAX = 4.0;
	// �ړ��ʉ��Z�l
	constexpr auto ADD_VECTOR = 0.15;
	// ����^�����Ă���̑ҋ@����
	constexpr auto GIVE_STAY = 120;
	// ��j���̑ҋ@����
	constexpr auto BREAK_STAY = 30;
}

namespace inr {

	BigDoll::BigDoll(Game& game) : EnemyBase(game) {
		// �e�평����
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
	// ������
	void BigDoll::Init() {
		// �����蔻��ݒ�
		_mainCollision = { _position, enemy::BIG_WIDTH / 2, enemy::BIG_HEIGHT / 2, true };
		// ���G�͈͂̐ݒ�
		_searchBox = { _position, 260, 260, enemy::BIG_HEIGHT / 2 + 30, enemy::BIG_HEIGHT / 2, true};
		// �����蔻��̐ݒ�
		_collisions = {
			// �����k��Ԏ��̔���
			{enemy::BIG_EMPTY, {_position, BIG_EMPTY_WIDTH / 2, BIG_EMPTY_WIDTH / 2, 0, BIG_EMPTY_HEIGHT, true}},
		};
		// �e�탂�[�V�����̉摜�L�[�A�����[�V�����t���[�����ASE�̍Đ����Ԃ̐ݒ�
		_motionKey = {
			// �����k
			{ enemy::BIG_EMPTY, {7 * 3, 0}},
			// �Ԃ����������Ă�����
			// �N���オ��
			{ enemy::red::BIG_WAKEUP, {enemy::red::big::WAKEUP_SIZE * 4, 0}},
			// �ҋ@
			{ enemy::red::BIG_IDOL, {BIG_STAY, 0}},
			// ����
			{ enemy::red::BIG_PATROL, {enemy::red::big::MOVE_SIZE * 4, 0}},
			// �^�b�N��
			{ enemy::red::BIG_TACKLE, {enemy::red::big::ATTACK_SIZE * 4, 0}},
			// �q�b�v�h���b�v
			{ enemy::red::BIG_HIPDROP, {enemy::red::big::HIPDROP_SIZE * 3, 0}},

			// �����������Ă�����
			// �N���オ��
			{ enemy::blue::BIG_WAKEUP, {enemy::blue::big::WAKEUP_SIZE * 4, 0}},
			// �ҋ@
			{ enemy::blue::BIG_IDOL, {BIG_STAY, 0}},
			// ����
			{ enemy::blue::BIG_PATROL, {enemy::blue::big::PATROL_SIZE * 5, 0} },
			// ����
			{ enemy::blue::BIG_ESCAPE, {enemy::blue::big::ESCAPE_SIZE * 4, 50}},
		};
		_moveCount = 0;
	}
	// �X�V
	void BigDoll::Process() {
		ObjectBase::Process();	// �d�͏���
		_moveVector = { 0, 0 };	// �ړ��ʏ�����
		AnimationCount();		// �A�j���[�V�����J�E���^����
		Action();				// �A�N�V������ԂɈڍs���邩
		StateUpdate();			// ��Ԃɉ����������̍X�V
		Move();					// �ړ�����
		Attack();				// �U������
		PositionUpdate();		// ���W�X�V
	}
	// �A�N�V������ԂɈڍs���邩 
	void BigDoll::Action() {
		// �N���オ���Ԃ܂��́A�A�N�V�������s���̏ꍇ�͏����𔲂���
		if (_aState == ActionState::WAKEUP || _isAction == true) return;
		// �v���C���[�𔭌��ł��邩
		if (SearchPlayer() == true) {
			// ���������ꍇ�͔����G�t�F�N�g�̐�������ѓo�^
			auto eyelight = std::make_unique<EffectBase>(_game.GetGame(), effect::bigdoll::OMEN, _position, effect::bigdoll::OMEN_NUMS * 3, _direction);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eyelight), effect::type::FORMER);
			// ���̐F�ɉ�����SE��炷
			switch (_soul->SoulColor()) {
			case soul::RED:
				PlaySe(enemy::bigdoll::SE_TACKLE_VOICE);
				break;
			case soul::BLUE:
				PlaySe(enemy::bigdoll::SE_ESCAPE_VOICE);
				break;
			}
			_isAction = true;	// �A�N�V�����J�n
			ChangeIdol(BIG_STAY * 2);	// ��莞�Ԃ̊ԁA�ҋ@��ԂɑJ��
		}
		// �����ł��Ȃ������ꍇ�͈ړ��������s��
		if (_soul == nullptr) _actionX = 0;
	}
	// �U������
	void BigDoll::Attack() {
		if (_soul == nullptr || _aState == ActionState::WAKEUP) return;	// ������ or �N���オ���Ԃ̏ꍇ�͏������s��Ȃ�
		auto&& player = _game.GetObjectServer()->GetPlayer();	// ���@�̎擾
		auto collision = player->GetMainCollision();	// �v���C���[�̓����蔻��
		// �U���܂��͓�����Ԃ̏ꍇ
		if (_aState == ActionState::ATTACK || _aState == ActionState::ESCAPE) {
			auto gs = _game.GetGimmickServer()->GetGimmicks();
			for (auto gg : gs) {
				// �h�A�܂��͐����̏ꍇ�͏������X�L�b�v
				if (gg->GimmickType() == gimmick::DOOR || gg->GimmickType() == gimmick::CRYSTAL) continue;	
				// �Փ˔��肪�^�̏ꍇ
				if (_mainCollision.HitCheck(gg->GetMainCollision())) {
					switch (gg->GimmickType()) {
					// �u���b�N�̏ꍇ
					case gimmick::BLOCK:
						// �Ώۂ��j�󂳂�Ă��Ȃ��ꍇ
						if (std::dynamic_pointer_cast<Block>(gg)->IsBreak() != gimmick::block::BRAKE_ON) {
							_stay = 30;	// �ҋ@�J�E���^�̐ݒ�
							std::string key = "";
							if (_soul->SoulColor() == soul::BLUE) key = enemy::blue::BIG_IDOL;
							else key = enemy::red::BIG_IDOL;
							ChangeState(ActionState::IDOL, key);
							std::dynamic_pointer_cast<Block>(gg)->Break(); 
						}
						continue;
						// ���o�[�̏ꍇ
					case gimmick::LEVER:
						// �U����Ԃ̏ꍇ�̂ݏ��������s
						if (_aState != ActionState::ATTACK) continue;
						// �����������
						std::dynamic_pointer_cast<Lever>(gg)->OpenDoor();
						continue;
					default:
						continue;
					}
				}
			}
		}
		// ���g�̓����蔻��ƐڐG������s��
		auto dmb = DamageBox(10);
		if (dmb.HitCheck(collision)) {
			player->Damage(SearchPosition());
			return;
		}
	}
	// �q�b�v�h���b�v
	void BigDoll::HipDrop() {

	}
	// ��Ԃɉ����������̍X�V
	void BigDoll::StateUpdate() {
		// ������̏ꍇ�͏����𒆒f
		if (_soul == nullptr) return;
		// �ҋ@�J�E���^������ꍇ
		if (0 < _stay) {
			--_stay;	// �ҋ@�J�E���^�����炷
			// �ҋ@�J�E���^�������Ȃ����ꍇ
			if (_stay == 0) {
				// �_���[�W������I���ɂ���
				_mainCollision.SetCollisionFlag() = true;
			}
			return;
		}
		// ����SE�̃C���^�[�o��������ꍇ
		if (0 < _moveCount) {
			--_moveCount;
			// �U���E������Ԃł͂Ȃ��ꍇ�A�C���^�[�o��������������
			if (_aState != ActionState::ATTACK && _aState != ActionState::ESCAPE) _moveCount = 0;
		}
		switch (_aState) {
			// �ҋ@
		case ActionState::IDOL:
			// �_���[�W���肪�U�̏ꍇ�A�����^�ɂ���
			if (_mainCollision.GetCollisionFlag() != true) _mainCollision.SetCollisionFlag() = true;
			// �A�N�V�����t���O���^�̏ꍇ
			if (_isAction == true) {
				// ���̐F�ɉ������A�N�V�������I������
				switch (_soul->SoulColor()) {
				case soul::RED:
					AttackOn();
					_isAction = false;
					return;
				case soul::BLUE:
					EscapeOn();
					_isAction = false;
					return;
				}
			}
			// �A�N�V�����t���O���U�̏ꍇ�͏��񏈗����J�n
			PatrolOn();
			// ���G�����^�ɂ���
			_searchBox.SetCollisionFlag() = true;
			return;
			// ����
		case ActionState::PATROL:
			// ���ړ�
			if (_patrolX < 0) {
				// �ړ��ʂ���яI�������̍X�V
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				if (0 < _patrolX) _patrolX = 0;
				return;
			}
			// �E�ړ�
			else if (0 < _patrolX) {
				// �ړ��ʂ���яI�������̍X�V
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				if (_patrolX < 0) _patrolX = 0;
				return;
			}
			// �ړ��ʂ�0�̏ꍇ�͑ҋ@��ԂɑJ�ڂ���
			_aState = ActionState::IDOL;
			// ���ɉ������L�[�ɐ؂�ւ���
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = enemy::red::BIG_IDOL;
				break;
			case soul::BLUE:
				_divKey.first = enemy::blue::BIG_IDOL;
				break;
			}
			_stay = STAY_MAX;	// �ҋ@�J�E���^�ݒ�
			_searchBox.SetCollisionFlag() = false;	// ���G�����؂�
			return;
			// �U��	
		case ActionState::ATTACK:
			// �����Ă��Ă��A�ړ�SE�C���^�[�o�����Ȃ��ꍇ�̂�SE��炷
			if (_stand == true && _moveCount == 0) { 
				PlaySe(enemy::bigdoll::SE_TACKLE);
				_moveCount = 30;
			}
			// ���ړ�
			if (_actionX < 0) {
				// �ړ��ʂ���яI�������̍X�V
				if(_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX += _atkVec;
				_moveVector.GetPX() = -_atkVec;
				if (0 <= _actionX) _actionX = 0;
			// �E�ړ�
			} else if (0 < _actionX) {
				// �ړ��ʂ���яI�������̍X�V
				if (_atkVec < ATTACK_VECTOR_MAX) _atkVec += ADD_VECTOR;
				_actionX -= _atkVec;
				_moveVector.GetPX() = _atkVec;
				if (_actionX <= 0) _actionX = 0;
			}
			// �ړ����I�������ꍇ�͑ҋ@��Ԃ̐ݒ���s��
			if(_actionX == 0) ChangeIdol(BIG_STAY * 3);
			return;
			// ����
		case ActionState::ESCAPE:
			// �����Ă��Ă��A�ړ�SE�C���^�[�o�����Ȃ��ꍇ
			if (_stand == true && _moveCount == 0) {
				PlaySe(enemy::bigdoll::SE_TACKLE);
				_moveCount = 30;
			}
			// ���ړ�
			if (_actionX < 0) {
				// �ړ��ʂ���яI�������̍X�V
				_actionX += (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = -(enemy::ESCAPE_VECTOR / 30);
				if (0 < _actionX) _actionX = 0;
			// �E�ړ�
			} else if (0 < _actionX) {
				// �ړ��ʂ���яI�������̍X�V
				_actionX -= (enemy::ESCAPE_VECTOR / 30);
				_moveVector.GetPX() = (enemy::ESCAPE_VECTOR / 30);
				if (_actionX < 0) _actionX = 0;
			}
			// �ړ����I�������ꍇ�͑ҋ@��Ԃ̐ݒ���s��
			if (_actionX == 0) ChangeIdol(BIG_STAY * 3);
			return;
			// �N���オ�胂�[�V������
		case ActionState::WAKEUP:
			// �N���オ��A�j���[�V�����̍Đ����I������Ȃ�A�����ԂɑJ�ڂ���
			if (IsAnimationMax()) {
				ChangeIdol(BIG_STAY * 2);
				_stay = STAY_MAX;
#ifdef _DEBUG
				_searchBox.SetDrawFlag() = true;
#endif
			}
			return;
		}
	}
	// �ړ�����
	void BigDoll::Move() {
		// �ړ��ʂɉ����Č������X�V����
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}
	// ���S����
	void BigDoll::Death() {
		// ���S��ԂɑJ��
		ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
		// ���S�����Ăяo��
		EnemyBase::Death();
	}
	// �I�u�W�F�N�g���̓o�^
	void BigDoll::SetParameter(ObjectValue objValue) {	
		// ���N���X�̓o�^�����Ăяo��
		EnemyBase::SetParameter(objValue);
		// ������̏ꍇ�͔����k�ɂȂ�
		if (_oValue.SoulType() == 0) {
			// �����k���̓����蔻����X�V
			auto embox = _collisions.find(enemy::BIG_EMPTY);
			embox->second.Update(_position, _direction);
			// �����k��ԂɑJ��
			ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
			// �e�평����
			_aCount = AnimationCountMax();
			_changeGraph = true;
			_mainCollision.SetCollisionFlag() = false;
			return;	// �����𔲂���
		}
		// ���̐���
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());
		// �o�^���ɉ������ݒ���s��
		switch (_oValue.SoulType()) {
		case 1:
			soul->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::red::BIG_PATROL);
			break;
		case 2:
			soul->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::blue::BIG_PATROL);
			break;
		default:
#ifdef _DEBUG
			OutputDebugString("error�FEnemyBase->SetParameter�@ObjectValue��_soulType�̒l���s���ł�\n");
#endif
			break;
		}
		// ���̓o�^
		_soul = soul;
		_game.GetObjectServer()->Add(std::move(soul));
	}
	// ���W�X�V
	void BigDoll::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		_moveVector.GetPY() = _gravity;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction);
		GimmickCheck(_moveVector);	// �M�~�b�N�Ƃ̓����蔻��
		_position = _position + _moveVector;	// �ʒu���W���X�V
		// �e�퓖���蔻��̍X�V
		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);
		auto col = _collisions.find(enemy::BIG_EMPTY);
		col->second.Update(_position, _direction);
		if (_soul == nullptr && IsAnimationMax() == true) {
			_mainCollision.Swap(col->second);
#ifdef _DEBUG
			_searchBox.SetDrawFlag() = false;
#endif
		}
	}
	// ���񏈗�����I��
	void BigDoll::PatrolOn() {
		// ������̏ꍇ�͏����𒆒f
		if (_soul == nullptr) return;
		// ���N���X�̏����Ăяo��
		EnemyBase::PatrolOn();

		// ���̐F�ɉ����ăL�[��؂�ւ�
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::BIG_PATROL : _divKey.first = enemy::red::BIG_PATROL;
		// �ړ����I�����Ă���ꍇ
		if (_actionX == 0) {
			// ���݂̌����Ƃ͔��Ε����ւ̈ړ���ݒ�
			if (_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}
	// �U����������I��
	void BigDoll::AttackOn() {
		// �U����Ԃł͂Ȃ��ꍇ
		if (_aState != ActionState::ATTACK) {
			// �U����ԂɑJ�ڂ���
			ChangeState(ActionState::ATTACK, enemy::red::BIG_TACKLE);
			_searchBox.SetCollisionFlag() = false;
			PlaySe(enemy::bigdoll::SE_TACKLE_VOICE);
			// ���݌����Ă�������Ɉړ���ݒ�
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
	// ���݂̓����蔻��̎擾
	AABB BigDoll::NowCollision(std::string key) {
		// ������ł͂Ȃ��ꍇ�͒ʏ�̓����蔻���Ԃ�
		if (_soul != nullptr) return _mainCollision;
		// ��̏ꍇ�͔����k��Ԃ̓����蔻���Ԃ�
		auto it = _collisions.find(enemy::BIG_EMPTY);
		return it->second;
	}
	// ������������I��
	void BigDoll::EscapeOn() {
		// ������Ԃł͂Ȃ��ꍇ
		if (_aState != ActionState::ESCAPE) {
			// ������ԂɈڍs
			ChangeState(ActionState::ESCAPE, enemy::blue::BIG_ESCAPE);
			_searchBox.SetCollisionFlag() = false;
			PlaySe(enemy::bigdoll::SE_ESCAPE_VOICE);
			// ���݌����Ă�������Ƃ͔��Ε����Ɉړ���ݒ�
			switch (_direction) {
			case enemy::MOVE_LEFT:
				_actionX = enemy::ESCAPE_MAX;
				return;
			case enemy::MOVE_RIGHT:
				_actionX = -enemy::ESCAPE_MAX;
				return;
			}
		}
	}
	// �ҋ@��Ԃ̐ݒ�
	void BigDoll::ChangeIdol(int stay) {
		_stay = stay;	// �ҋ@����
		_atkVec = 0;	// �U���ړ��ʂ���ɂ���
		_searchBox.SetCollisionFlag() = false;	// �����蔻����U�ɂ���
		std::string nextkey = "";
		// ���ɉ������L�[�̐ݒ�
		switch (_soul->SoulColor()) {
		case soul::RED:
			nextkey = enemy::red::BIG_IDOL;
			break;
		case soul::BLUE:
			nextkey = enemy::blue::BIG_IDOL;
			break;
		}
		// �ҋ@��ԂɑJ��
		ChangeState(ActionState::IDOL, nextkey);
	}
	// ���@�A�N�V�����Ƃ̏Փ˔���
	void BigDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// �}��(�����D���锻��͈�)���Z�o
		auto vitalpart = VitalPart(_mainCollision, BIG_DOLL_VITAL);
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���@�͒D���A�N�V���������H
		if (ckey == PKEY_ROB) {
			// ���͋󂩂ǂ����H
			if (_soul != nullptr) {
				// ���@�ƌ������������A�Փ˔��肪����ꍇ
				if (_direction == direction && vitalpart.HitCheck(acollision)) {
					// �����k�Ɉڍs
					ChangeState(ActionState::EMPTY, enemy::BIG_EMPTY);
					_searchBox.SetCollisionFlag() = false;
					PlaySe(enemy::bigdoll::SE_DOWN);
					// ���S�G�t�F�N�g�̐�������ѓo�^
					auto hit = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy::S_HIT, _position, 30);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hit), effect::type::FORMER);

					_soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������
					_moveCount = 0;
					_stay = 0;
					_isAction = false;
					_mainCollision.SetCollisionFlag() = false;	// �����蔻���؂�
					// ���@���ۗL���鍰����������ɓ��B���Ă���ꍇ�͏��L���������
					if (player->IsSoulMax()) {
						_soul->OwnerNull();	// ���L�҂����Z�b�g
						_soul.reset();	// ���L���������
						return;
					}
					player->SoulCatch(std::move(_soul));	// ���̏��L�����v���C���[�ɏ��n
					return;
				}
			}
		}
		// ���@�͗^����A�N�V���������H
		if (ckey == PKEY_GIVE) {
			// �v���C���[�͍����������Ă��邩�H
			if (player->HaveSoul()) {
				// ���g�̍�����̏ꍇ
				if (_soul == nullptr) {
					// �Փ˔���i�D���A�N�V�����Ƃ͈Ⴂ�A�����ɂ�鐧���Ȃ��j
					if (NowCollision(_divKey.first).HitCheck(acollision)) {
						auto player = _game.GetObjectServer()->GetPlayer();
						_soul = player->GiveSoul();	// �v���C���\����Ώۂ̍����󂯎��
						_soul->Inactive();	// ����񊈐���Ԃɂ���
						PlaySe(key::SOUND_PLAYER_GIVE_TRUE);
						_searchBox.SetCollisionFlag() = false;

						// �^����ꂽ���ɉ������A�N���オ���ԂɑJ�ڂ���
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