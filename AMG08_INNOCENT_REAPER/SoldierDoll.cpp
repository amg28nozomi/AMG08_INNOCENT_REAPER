#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "MapChips.h"
#include "Player.h"
#include "GimmickServer.h"
#include "GimmickBase.h"
#include "Lever.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "EffectBase.h"
#include "EffectServer.h"
#include "ModeServer.h"
#include "ModeMain.h"

// ����
#include "SoulSkin.h"
#include <memory>

namespace {
	constexpr auto START_POS_X = 500;
	constexpr auto START_POS_Y = 400;

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
	constexpr auto SEARCH_X = 200 + (SOLDIER_BOX_W * 2);	//�@260
	constexpr auto SEARCH_Y1 = SOLDIER_BOX_H1;
	constexpr auto SEARCH_Y2 = SOLDIER_BOX_H2;

	// ����͈�(�ő�Vector)
	constexpr auto PATROL_VECTOR = 1.0;
	constexpr auto PATROL_MAX = 280;

	// �U���p�͈�
	constexpr auto ATTACK_W1 = SOLDIER_BOX_W + 30;
	constexpr auto ATTACK_W2 = SOLDIER_W / 2;
	constexpr auto ATTACK_H = 30;

	constexpr auto FRAME = 30;

	// �A�C�h�����[�V��������
	constexpr auto STAY_MAX = 60;	//�@stay
	constexpr auto GIVE_STAY = 33 * 2;

	constexpr auto IDOL_FRAME = 33;
}

namespace inr {

	SoldierDoll::SoldierDoll(Game& game) : EnemyBase(game){
		_type = ObjectBase::ObjectType::ENEMY;
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair(enemy::SOLDIER_EMPTY, key::SOUND_NUM);
		_position = { 0, 0 };

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_patrolX = 0;
		_aInterval = 0;
		_direction = false;
		_changeGraph = true;
		_drawStop = false;

		Init();
	}

	SoldierDoll::~SoldierDoll() {
	}

	void SoldierDoll::Init() {
		// ���C���̓����蔻���������
		// _mainCollision = { _position, SOLDIER_BOX_W, SOLDIER_BOX_W, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_mainCollision = { _position, 30, 60, true };

		// _searchBox = { _position, SEARCH_X, SEARCH_X, SOLDIER_BOX_H1, SOLDIER_BOX_H2, true };
		_searchBox = { _position, SEARCH_X, SEARCH_X, 70, 60, true };

		/*_searchBox = { { _mainCollision.GetMin().GetX() - SEARCH_X, _mainCollision.GetMin().GetY() - SEARCH_Y }, {
						_mainCollision.GetMax().GetX() + SEARCH_X, _mainCollision.GetMax().GetY() + SEARCH_Y } };*/
		// �e�픻��̊i�[�i�U������E�j
		_collisions = {
			{ enemy::SOLDIER_EMPTY, { _position, 60, 60, -25, 60 , true}},	// �����k���̏��
			// { enemy::SOLDIER_EMPTY, { _position, 65, 60, -45, 80 , true}},	// �����k���̏��
			{ enemy::red::SOLDIER_ATTACK, {_position, ATTACK_W1, ATTACK_W2, ATTACK_H, ATTACK_H, true}},
		};

		_motionKey =
		{	{ enemy::SOLDIER_EMPTY, {19 * 3, 0}},

			{ enemy::red::SOLDIER_WAKEUP, {17 * 3, 0}},
			{ enemy::red::SOLDIER_IDOL, {11 * 3, 0}},
			{ enemy::red::SOLDIER_PATROL, {13 * 3, 13 * 3}},
			{ enemy::red::SOLDIER_ATTACK, {11 * 3, 50}},	// SE�L��

			{ enemy::blue::SOLDIER_WAKEUP, {17 * 3, 0}},
			{ enemy::blue::SOLDIER_IDOL, {11 * 3, 0}},
			{ enemy::blue::SOLDIER_PATROL, {13 * 3, 13 * 3}},
			{ enemy::blue::SOLDIER_ESCAPE, {13 * 3, 0}}
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
		Attack();

		// �����蔻����擾

		Move();
		PositionUpdate();
	}

	void SoldierDoll::StateUpdate() {
		if (_soul == nullptr) return;	// �����Ȃ��ꍇ�͏������I��
		switch (_soul->SoulColor()) {
		case soul::RED:
			if (_aInterval) {
				--_aInterval;
			}
			return;
		case soul::BLUE:
			if (_aInterval) {
				--_aInterval;
			}
			return;
		}
	}

	void SoldierDoll::Action() {
		if (_aState == ActionState::WAKEUP || _isAction == true) return;
		// �v���C���[�𔭌��ł��邩
		if (SearchPlayer() == true) {
			_isAction = true;
			ChangeIdol(IDOL_FRAME);

			auto eye_light = std::make_unique<EffectBase>(_game.GetGame(), effect::soldier::OMEN, _position, effect::soldier::OMEN_NUMS * 3, _direction);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eye_light), effect::type::FORMER);
			switch (_soul->SoulColor()) {
			case soul::RED:
				PlaySe(enemy::soldier::ATTACK_VOICE);
				break;
			case soul::BLUE:
				PlaySe(enemy::soldier::ESCAPE_VOICE);
				break;
			}
		}
		// �����ł��Ȃ������ꍇ�͈ړ��������s��
		if (_soul == nullptr) _actionX = 0;
	}

	void SoldierDoll::Patrol() {
		if (_soul == nullptr) return;	// ������̏ꍇ�͏������s��Ȃ�
		if (_stay != 0) {
			--_stay;	// �ҋ@���[�V�������̓J�E���^�����炵�ď����𔲂���
			if (_stay == 0) { 
				_mainCollision.SetCollisionFlag() = true;
			}
			return;
		}

		switch (_aState) {
		case ActionState::IDOL:
			if (_mainCollision.GetCollisionFlag() != true) _mainCollision.SetCollisionFlag() = true;	
			if (_isAction == true) {
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
			PatrolOn();
			_searchBox.SetCollisionFlag() = true;
			return;
		case ActionState::PATROL:
			// ���ړ�
			if (_patrolX < 0) {
				_moveVector.GetPX() = PATROL_VECTOR;
				_patrolX += PATROL_VECTOR;
				return;
			// �E�ړ�
			} else if (0 < _patrolX) {
				_moveVector.GetPX() = -PATROL_VECTOR;
				_patrolX -= PATROL_VECTOR;
				return;
			}
			// �ړ��ʂ�0�̏ꍇ�͑ҋ@��ԂɑJ�ڂ���
			_aState =  ActionState::IDOL;
			switch (_soul->SoulColor()) {
			case soul::RED:
				_divKey.first = enemy::red::SOLDIER_IDOL;
				break;
			case soul::BLUE:
				_divKey.first = enemy::blue::SOLDIER_IDOL;
				break;
			}
			_stay = STAY_MAX;
			return;
		// �N���オ�胂�[�V������
		case ActionState::WAKEUP:
			// �N���オ��A�j���[�V�����̍Đ����I������Ȃ�A�����ԂɑJ�ڂ���
			if (AnimationCountMax() == true) {
				ChangeIdol(GIVE_STAY);
#ifdef _DEBUG
				_searchBox.SetDrawFlag() = true;
#endif
			}
			return;
		}


		// ������̏ꍇ�̓[���ɂ���
		//if (_aState == ActionState::EMPTY) _patrolX = 0;
		//else if (_aState != ActionState::PATROL && _aState != ActionState::ESCAPE && _aState!= ActionState::IDOL) {
		//	PatrolOn();
		//}
		//if (_aState == ActionState::PATROL) {
		//	// ���ړ�
		//	if (_patrolX < 0) {
		//		_moveVector.GetPX() = PATROL_VECTOR;
		//		_patrolX += PATROL_VECTOR;
		//		return;
		//	}
		//	else if (0 < _patrolX) {
		//		_moveVector.GetPX() = -PATROL_VECTOR;
		//		_patrolX -= PATROL_VECTOR;
		//	}
		//	// �ړ��ʂ�0�ɂȂ����i�[�ɕt�����j�ꍇ�͑ҋ@��ԂɑJ��
		//	if (static_cast<int>(_patrolX) == 0) {
		//		ActionState::IDOL;
		//		switch (_sState) {
		//		case SoulState::BLUE:
		//			_divKey.first = enemy::blue::SOLDIER_IDOL;
		//			break;
		//		case SoulState::RED:
		//			_divKey.first = enemy::red::SOLDIER_IDOL;
		//			break;
		//		}
		//		_stay = STAY_MAX;

		//		if (_direction == enemy::MOVE_LEFT) _patrolX = -PATROL_MAX;
		//		else _patrolX = PATROL_MAX;
		//	}
		//}
	}

	void SoldierDoll::Move() {
		// �ړ��x�N�g���ɉ����āA������ύX
		// �ړ��ʂɉ����Č�����ύX
		

		if (_actionX < 0) { 
			_direction = enemy::MOVE_LEFT; }
		else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;
		/*if (_aState == ActionState::PATROL && _aCount % GetSoundFrame(_divKey.first) == 0 && IsAnimationMax() != true) {
			PlaySe(enemy::soldier::MOVE);
		}*/

		

		Escape();

		/*if (_moveVector.GetX() < 0) {
			_direction = true;
		}
		else if (0 < _moveVector.GetX()) {
			_direction = false;
		}*/
	}

	void SoldierDoll::Escape() {
		switch (_aState) {
		case ActionState::PATROL:
			if (0 < _patrolX) _direction = enemy::MOVE_LEFT;
			else if (_patrolX < 0) _direction = enemy::MOVE_RIGHT;
			//if (_patrolX != 0) {
			//	switch (_direction) {
			//	case enemy::MOVE_LEFT:

			//		break;
			//	case enemy::MOVE_RIGHT:
			//		break;
			//	}
			//}
			return;

		case ActionState::ESCAPE:
			if (_aCount == 0 && _stand == true) PlaySe(enemy::soldier::ESCAPE_SE);
			if (_actionX < 0) {
				_direction = enemy::MOVE_LEFT;
			}
			else if (0 < _actionX) _direction = enemy::MOVE_RIGHT;


			if (_actionX != 0) {
				switch (_direction) {
				case enemy::MOVE_LEFT:
					_actionX += enemy::ESCAPE_VECTOR / FRAME;
					_moveVector.GetPX() = -enemy::ESCAPE_VECTOR / FRAME;
					break;
				case enemy::MOVE_RIGHT:
					_actionX -= enemy::ESCAPE_VECTOR / FRAME;
					_moveVector.GetPX() = enemy::ESCAPE_VECTOR / FRAME;
					break;
				}
				if (_actionX == 0) { 
					ChangeIdol(IDOL_FRAME);
				}
			}
			return;
		case ActionState::ATTACK:
			if (_aCount == 0) PlaySe(enemy::soldier::ATTACK_SE);
			if (_actionX < 0) {
				_direction = enemy::MOVE_RIGHT;
			}
			else if (0 < _actionX) _direction = enemy::MOVE_LEFT;

			if (_actionX != 0) {
				double mv = 0;
				switch (_direction) {
				case enemy::MOVE_LEFT:
					_actionX -= enemy::ATTACK_VECTOR / FRAME;
					mv = -enemy::ATTACK_VECTOR / FRAME;
					if (_actionX < 0) _actionX = 0;
					break;
				case enemy::MOVE_RIGHT:
					_actionX += enemy::ATTACK_VECTOR / FRAME;
					mv = enemy::ATTACK_VECTOR / FRAME;
					if (0 < _actionX) _actionX = 0;
					break;
				}
				_moveVector.GetPX() = mv;
				if (_actionX == 0) { 
					ChangeIdol(IDOL_FRAME);
				}
			}
			return;
		default:
			return;
		}
	}

	void SoldierDoll::PatrolOn() {
		if (_soul == nullptr) return;
		EnemyBase::PatrolOn();
		StopSound();

		// ���̐F�ɉ����ăL�[��؂�ւ�
		(_soul->SoulColor() == soul::BLUE) ?
			_divKey.first = enemy::blue::SOLDIER_PATROL : _divKey.first = enemy::red::SOLDIER_PATROL;

		if (_actionX == 0) {
			if(_direction) _patrolX = -PATROL_MAX;
			else _patrolX = PATROL_MAX;
		}
	}

	void SoldierDoll::AttackOn() {
		if (_aState != ActionState::ATTACK) {
			ChangeState(ActionState::ATTACK, enemy::red::SOLDIER_ATTACK);
			(_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
			_searchBox.SetCollisionFlag() = false;	// �A�N�V�����ɓ˓�������ꎞ�I�ɍ��G������؂�
		}
	}

	void SoldierDoll::EscapeOn() {

		// ������Ԃł͂Ȃ��ꍇ�̂݁A
		if (_aState != ActionState::ESCAPE) {
			ChangeState(ActionState::ESCAPE, enemy::blue::SOLDIER_ESCAPE);
			(_direction == enemy::MOVE_LEFT) ? _actionX = enemy::ESCAPE_MAX : _actionX = -enemy::ESCAPE_MAX;
			_searchBox.SetCollisionFlag() = false;	// �A�N�V�����ɓ˓�������ꎞ�I�ɍ��G������؂�
		}
	}

	void SoldierDoll::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		_moveVector.GetPY() = _gravity;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		if (_soul != nullptr) {
			int i = 0;
		}
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);
		GimmickCheck(_moveVector);
		_position = _position + _moveVector;	// �ʒu���W���X�V

		_mainCollision.Update(_position, _direction);
		_searchBox.Update(_position, _direction);

		if (_aState == ActionState::ATTACK) {
			auto it = _collisions.find(_divKey.first);
			it->second.Update(_position, _direction);
#ifdef _DEBUG
			if(it->second.SetDrawFlag() == false) it->second.SetDrawFlag() = true;
#endif
		}

		auto col = _collisions.find(enemy::SOLDIER_EMPTY);
		col->second.Update(_position, _direction);

		if (_soul == nullptr && IsAnimationMax() == true) {
			col->second.SetCollisionFlag() = true;
			// _mainCollision.Swap(col->second);
#ifdef _DEBUG
			_searchBox.SetDrawFlag() = false;
#endif
			
		}
	}

	void SoldierDoll::Attack() {
		if (_soul == nullptr || _aState == ActionState::WAKEUP) return;	// �����Ȃ��ꍇ�͏������X�L�b�v
		auto&& player = _game.GetObjectServer()->GetPlayer();
		auto playerBox = player->GetMainCollision();	// �v���C���[�̓����蔻����擾

		// �M�~�b�N�i���o�[�j�ƏՓ˂������H
		if (_aState == ActionState::ATTACK) {
			auto gs = _game.GetGimmickServer()->GetGimmicks();
			for (auto gg : gs) {
				if (gg->GimmickType() != gimmick::LEVER) continue;	// ���o�[�ł͂Ȃ��ꍇ�̓X�L�b�v
				if (_mainCollision.HitCheck(gg->GetMainCollision())) {
					std::dynamic_pointer_cast<Lever>(gg)->OpenDoor();
				}
			}
		}

		// ���g�̓����蔻��ƐڐG������s��
		auto dmb = DamageBox(10);
		if (dmb.HitCheck(playerBox)) {
			player->Damage(SearchPosition());
			return;
		}
		// �U����Ԃ̏ꍇ�͒ǉ��ŏ������s��
		if (_aState == ActionState::ATTACK) {
			auto box = _collisions.find(_divKey.first);
			// �ڐG���Ă���ꍇ�͎��@�̏Փˏ������Ăяo��
			if (box->second.HitCheck(playerBox)) {
				// �i�s�����Ƃ͓������ɑΏۂ��΂�
				player->Damage(SearchPosition());
			}
		}

		//if (_mainCollision.HitCheck(playerBox)) {
		//	player.Da
		//}
	}

	void SoldierDoll::ChangeIdol(int stay) {
		_stay = stay;
		_searchBox.SetCollisionFlag() = false;	// �����蔻���؂�
		std::string nextkey = "";
		switch (_soul->SoulColor()) {
		case soul::RED:
			nextkey = enemy::red::SOLDIER_IDOL;
			break;
		case soul::BLUE:
			nextkey = enemy::blue::SOLDIER_IDOL;
			break;
		}
		ChangeState(ActionState::IDOL, nextkey);
	}

	bool SoldierDoll::Hit() {
		// �v���C���[�̎Q�Ƃ��擾
		auto player = _game.GetObjectServer()->GetPlayer();

		// �v���C���[�͎��g�ɐڐG���Ă��邩�ǂ����H
		if (_mainCollision.HitCheck(player->GetMainCollision())) {
			player->GetMainCollision();
		}
		return false;
	}


	void SoldierDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		// ���݂̋}����������W���Z�o
		auto vitalPart = VitalPart(_mainCollision, SOLDIER_VITAL);
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���͒D���邩�H
		if (ckey == PKEY_ROB) {
			if (_soul != nullptr) {
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// ����D����
					ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
					StopSound();
					PlaySe(enemy::soldier::DOWN);
					_searchBox.SetCollisionFlag() = false;	// �ꎞ�I�ɍ��G�����؂�
					_mainCollision.SetCollisionFlag() = false;
					_soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������
					_stay = 0;
					_isAction = false;

					auto hiteff = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy:: S_HIT, _position, 30);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(hiteff), effect::type::FORMER);

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
			if(player->HaveSoul()){
				// ������̏ꍇ�Ƀ{�b�N�X���ڐG������
				if (_soul == nullptr) {
					// �ڐG���̔����AABB�ōs���i�D���A�N�V�����Ƃ͈Ⴂ�A�����ɂ�鐧���Ȃ��j
					if (NowCollision(_divKey.first).HitCheck(acollision)) {
						// �v���C���[���擾
						auto player = _game.GetObjectServer()->GetPlayer();
						_soul = player->GiveSoul();	// �v���C���\����Ώۂ̍����󂯎��
						_soul->Inactive();	// ����񊈐���Ԃɂ���
						PlaySe(key::SOUND_PLAYER_GIVE_TRUE);
						_searchBox.SetCollisionFlag() = false;
						switch (_soul->SoulColor()) {
						case soul::RED:
							ChangeState(ActionState::WAKEUP, enemy::red::SOLDIER_WAKEUP);
							return;
						case soul::BLUE:
							ChangeState(ActionState::WAKEUP, enemy::blue::SOLDIER_WAKEUP);
							return;
						}
						return;
					}
				}
			}
		}
	}

	void SoldierDoll::SetParameter(ObjectValue objValue) {
		EnemyBase::SetParameter(objValue);
		if (_oValue.SoulType() == 0) {	// ������̏ꍇ�͔����k�ɂȂ�
			auto embox = _collisions.find(enemy::SOLDIER_EMPTY);
			embox->second.Update(_position, _direction);
			ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
			_aCount = AnimationCountMax();	// �J�E���^���}�b�N�X�ɂ���
			_searchBox.SetCollisionFlag() = false;	// �ꎞ�I�ɍ��G�����؂�
			_mainCollision.SetCollisionFlag() = false;
			_changeGraph = false;
			return;	// �����𔲂���
		}
		auto soul_n = std::make_shared<SoulSkin>(_game.GetGame());
		// auto sn = std::static_pointer_cast<SoulSkin>(soul_n);
		switch (_oValue.SoulType()) {
		case 1:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::red::SOLDIER_PATROL);
			break;
		case 2:
			soul_n->SetParameter(_oValue.SoulType(), 7);
			ChangeState(ActionState::PATROL, enemy::blue::SOLDIER_PATROL);
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

	void SoldierDoll::Death() {
		ChangeState(ActionState::EMPTY, enemy::SOLDIER_EMPTY);
		EnemyBase::Death();
		PlaySe(enemy::soldier::DOWN);
	}

	AABB SoldierDoll::NowCollision(std::string key) {
		if(_soul != nullptr) return _mainCollision;
		auto it = _collisions.find(enemy::SOLDIER_EMPTY);
		// ���݂̃A�N�V������Ԃ̓{�b�N�X���C������K�v�����邩�H
		return it->second;
	}

	bool SoldierDoll::StopSound() {
		auto attack = se::SoundServer::GetSound(enemy::soldier::ATTACK_SE);
		auto escape = se::SoundServer::GetSound(enemy::soldier::ESCAPE_SE);
		if (CheckSoundMem(attack) == 1) StopSoundMem(attack);
		if (CheckSoundMem(escape) == 1) StopSoundMem(escape);
		return true;
	}
}