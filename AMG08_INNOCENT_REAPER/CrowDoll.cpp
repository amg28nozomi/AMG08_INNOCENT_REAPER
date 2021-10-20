/*****************************************************************//**
 * @file   CrowDoll.cpp
 * @brief  �{�X�G�l�~�[�̃N���E�h�[���N���X�i�G�l�~�[�x�[�X�N���X�̃T�u�N���X�j
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "TrackingEffect.h"
#include "LoopEffect.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "MapChips.h"
#include "ChipNumber.h"
#include "SoulSkin.h"
#include <random>
#include <time.h>
#include <memory>

namespace {
	// �N���E�h�[���̓����蔻��
	constexpr auto CROW_WIDTH = 60;
	constexpr auto CROW_HEIGHT = 260;
	// �}��
	constexpr auto CROW_VITAL = 20;
	// �{���Ԃւ̓˓����C��
	constexpr auto CROW_ANGER = 5;
	// ���V���̏��
	constexpr auto DEFAULT_Y = 400;
	// �{���Ԃ��ǂ����̃t���O
	constexpr auto IS_ANGER = 1;
	constexpr auto IS_NORMAL = 0;
	// �A���؂�(��a�蕪)�̑��ړ���
	constexpr auto RUSH_MAX = 250;
	// �A���؂�1�񕪂ɕK�v�ȃt���[����
	constexpr auto RUSH_FRAME_MAX = 15;
	// �ϋv�l�̏��
	constexpr auto LIFE_MAX = 1;
	// �f�o�t�����܂łɕK�v�ȗ��ߒi�K
	constexpr auto DEBUFF_COUNT_MIN = 0;
	constexpr auto DEBUFF_COUNT_MAX = 5;
	// �f�o�t�𔭓�����A�j���[�V�����ԍ�
	constexpr auto DEBUFF_START = 6;
	// �T�C�Y�C���l
	constexpr auto SIZE_FIX = 1;
}
namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _crowState(CrowState::SLEEP) {
		// �e��ݒ�
		_type = ObjectBase::ObjectType::ENEMY;	
		_aState = ActionState::IDOL;
		_eType = EnemyType::CROW_DOLL;
		_crowState = CrowState::SLEEP;
		Init();
	}

	void CrowDoll::Init() {
		// �e�평����
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		// �����蔻��
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	
		_collisions = {
			// �A���؂�̓����蔻��
			{enemy::crowdoll::CROW_RUSH, {_position, 210, -30, CROW_HEIGHT / 2 , CROW_HEIGHT / 2, true}},
			// �����k���̓����蔻��
			{enemy::crowdoll::CROW_DOWN, {_position, 80, 80, -70, CROW_HEIGHT / 2, true}},
		};
		// { �摜�����p�L�[, {���t���[����, SE�̍Đ�����} }
		_motionKey = {
			// �ҋ@���
			{enemy::crowdoll::CROW_IDOL, {enemy::crowdoll::motion::IDOL * 2, 0}},
			// �A���؂�
			{enemy::crowdoll::CROW_RUSH , {enemy::crowdoll::motion::RUSH  * 2, 20}},
			// �����U��
			{enemy::crowdoll::CROW_BLINK , {enemy::crowdoll::motion::BLINK * 2, 20}},
			// �L�т�r
			{enemy::crowdoll::CROW_GROWARM , {enemy::crowdoll::motion::GROWARM * 3, 20}},
			// ���K
			{enemy::crowdoll::CROW_ROAR , {enemy::crowdoll::motion::ROAR * 4, 50}},
			// �f�o�t
			{enemy::crowdoll::CROW_DEBUFF, {enemy::crowdoll::motion::DEBUF * 3, 50}},
			// �_�E��
			{enemy::crowdoll::CROW_DOWN , {enemy::crowdoll::motion::DOWN * 3, 50}},
			// ����
			{enemy::crowdoll::CROW_WINCE, {enemy::crowdoll::motion::WINCE * 5, 50}},
		};
		// ������
		_aCount = GetSize(_divKey.first) - SIZE_FIX;
		_atkInterval = 0;
		_actionCount = 0;
		_invincible = 0;
		_life = LIFE_MAX;
		_arm = false;
		_setup = false;
		_changeGraph = false;
		_wait = false;
		_direction = true;
		_isAnimation = false;
		_isWarp = false;
		_debuff = false;
		_debuffChage = DEBUFF_COUNT_MIN;
	}

	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;										// �o�^
		_position = _oValue.Positions()[0];		// ���W���X�V
		Init();																// ������
	}

	void CrowDoll::Process() {
		IsGravity();		// �d�͏���
		IsBattle();			// �{�X�퓬�t���O
		WakeUp();				// �N���オ��
		if (IsActive() != true) return;											// ������ԂłȂ��ꍇ�͏������s��Ȃ�
		if (_isWarp == true && _atkInterval == 0) Warp();		// ���[�v����
		SetState();			// ��Ԃ̐ݒ�
		Floating();			// ���V����
		Attack();				// �_���[�W����
		Move();					// �ړ�����
	}

	void  CrowDoll::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// ���W���N�����v����
		// �`����W�̎Z�o
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;	// �O���t�B�b�N�n���h���i�[�p

		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);	// �����蔻��̕`��
		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		// �e�픻��̕`��(�`�揈�����I���̏ꍇ�̂�)
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlag() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
#endif
	}

	void CrowDoll::WakeUp() {
		// �{�X�킪�n�܂��Ă��Ȃ��ꍇ�͏I��
		if (_game.GetModeServer()->GetModeMain()->BossFlag() != true) return;
		if (_setup == true) return;		// �������������Ă���ꍇ���I��
		--_aCount;										// �J�E���^���Z(�t�Đ�)
		if (_aCount == 0) {		// �J�E���^��0�ɂȂ����ꍇ
			_setup = true;							// �Z�b�g�A�b�v����
			_mainCollision.SetCollisionFlag() = true;		// �����蔻����I���ɂ���
			ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
			_isAnimation = true;		// �A�j���[�V�����Đ����J�n
			return;
		}
	}

	void CrowDoll::GetTarget() {
		_target = _game.GetObjectServer()->GetPlayer()->GetPosition();	// �ڕW�̌��ݒn�_���擾����
	}

	void CrowDoll::ChangeDirection() {
		// �ړ������ɉ����Č�����ύX����
		if (_moveVector.GetX() < 0) _direction = enemy::MOVE_LEFT;
		else if (0 < _moveVector.GetX()) _direction = enemy::MOVE_RIGHT;
	}
	// ��ԑJ��
	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;			// �摜�L�[�؂�ւ�
		_crowState = nextState;		// ��ԑJ��
		_changeGraph = true;			// �摜�؂�ւ��t���O�I��
	}
	// �ړ�����
	void CrowDoll::Move() {
		ChangeDirection();								// �����̒���
		_moveVector.GetPY() = _gravity;		// y���W�ɏd�͒l���Z
		// �����o������
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);

		_position = _position + _moveVector;						// ���W�X�V
		_mainCollision.Update(_position, _direction);		// �����蔻��̍X�V
		// �����蔻��̍X�V
		for (auto&& it : _collisions) it.second.Update(_position, _direction);
		_moveVector = { 0, 0 };		// �ړ��ʂ�����������
	}

	void CrowDoll::Attack() {
		// �U������
		auto player = _game.GetObjectServer()->GetPlayer();
		const auto& collision = player->GetMainCollision();		// �v���C���[�̓����蔻��

		// �܂��͓G�̓����蔻��ƐڐG������s��
		if (_mainCollision.HitCheck(collision) == true) {
			// �����������H
			player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// ���W�����ɔ�΂�
			return;		// �U�����肪�������ꍇ�͏������I������
		}
		
		if (_crowState != CrowState::RUSH) return;			// �U���{�b�N�X���Ȃ��ꍇ�͏������s��Ȃ�
		if (_atkInterval != 0) return;									// �҂����Ԓ��͔�����s��Ȃ�
		auto damage = _collisions.find(_divKey.first);	// �{�b�N�X�͂��邩�H
		if (damage == _collisions.end()) return;
		if(damage->second.HitCheck(collision) == true) player->Damage(IsPlayerPos(player->GetPosition().GetX()));		// ���W�����ɔ�΂�
	}

	void CrowDoll::IsGravity() {
		// ���݂̏�Ԃɉ����ďd�͏������s�����̔�����s��
		switch (_crowState) {
		case CrowState::DEATH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
		case CrowState::RUSH:
			// �`��摜���ҋ@��Ԃ̏ꍇ
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				_gravity = 0;			// �d�͒l������
				_stand = false;		// �����Ă��Ȃ�
				return;
			}
			_gravity += FRAME_G;											// �d�͒l���Z
			if (MAX_G < _gravity) _gravity = MAX_G;		// ����l�𒴉߂��Ă���ꍇ�͏C��
			if (IsStandChip()) {											// �n�ʂɗ����Ă��邩�H
				if (0 < _gravity) _stand = true;				// �����Ă���
				if (_crowState == CrowState::BLINK) {
					_gravity = 0;
					return;
				}
				_gravity = 0;
			}
			else {
				_stand = false;		// �����Ă��Ȃ�
			}
			break;
		default:	// �d�͏����͍s��Ȃ�
			if (0 < _gravity) _gravity = 0;		// �d�͒l���v���X�̏ꍇ��0�ɂ���
			return;
		}
	}

	void CrowDoll::Floating() {
		// ���ݏ�Ԃ̏ꍇ
		if (_crowState == CrowState::WINCE) {
			// ���݂�Y���W�����V����ȉ��̏ꍇ
			if (_position.GetY() + _gravity <= DEFAULT_Y) {
				_position.GetPY() = DEFAULT_Y;	// ���W���C��
				_gravity = 0;										// �d�͒l������
				return;
			}
		}
		// �ҋ@��Ԃ܂��͙��K��Ԃ̏ꍇ
		if (_crowState == CrowState::IDOL || _crowState == CrowState::ROAR) {
			if (_position.GetY() <= DEFAULT_Y) return;		// �����ɓ��B���Ă���
			_gravity -= 0.25;															// �d�͒l���Z
			if (_position.GetY() + _gravity < DEFAULT_Y) {
				// �����ɓ��B�����ꍇ�͍��W���C������
				_position.GetPY() = DEFAULT_Y;
				_gravity = 0;
			}
			return;
		}
		return;
	}

	void CrowDoll::Warp() {
		_isWarp = false;						// ���[�v�t���O���I�t�ɂ���
		_isAnimation = true;				// �A�j���[�V�����ĊJ
		AddWarpEffect(_warpPos);		// �Ǐ]�����L��
		_position = _warpPos;				// ���W��؂�ւ���
		// ��Ԃɉ��������������s
		switch (_crowState) {
		case CrowState::RUSH:
			PlaySe(enemy::crowdoll::SE_RUSH);
			ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_RUSH);			// ��Ԑ؂�ւ�
			AddRushEffect();		// �A���G�t�F�N�g����
			return;
		case CrowDoll::CrowState::DEBUFF:
			ModeChange(CrowState::DEBUFF, enemy::crowdoll::CROW_DEBUFF);	// ��Ԑ؂�ւ�
			return;
		case CrowDoll::CrowState::ROAR:
			return;
		case CrowDoll::CrowState::BLINK:
			ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_BLINK);		// ��Ԑ؂�ւ�
			AddBlinkEffect();		// �����G�t�F�N�g����
			return;
		case inr::CrowDoll::CrowState::IDOL:
			return;
		default:
			return;
		}
	}

	void CrowDoll::Rash() {
		_isAnimation = true;	// �A�j���[�V�����Đ��J�n

		double mx = 0;
		double nextpos = 0;
		switch (_direction) {
		case enemy::MOVE_LEFT:		// �������̏ꍇ
			mx = -RUSH_MAX / RUSH_FRAME_MAX;	// �ړ��ʎZ�o
			_moveVector.GetPX() = mx;					// �ړ��x�N�g���ɑ��
			nextpos = _position.GetX() + mx;	// �ړ���̍��W���Z�o
			if (IsAttackEnd() == true) {			// �}�b�v�`�b�v�ɏՓ˂������H
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));	// SE���~�߂�
				return;
			}
			// �ړ����X���W���ڕW���W�ɖ����Ȃ��ꍇ�͏������I��
			if (_actionEnd.GetX() < nextpos) return;
			_actionEnd.GetPX() = nextpos - RUSH_MAX;	// �ڕW���W�̍X�V
			break;
		case enemy::MOVE_RIGHT:		// �E�����̏ꍇ
			mx = RUSH_MAX / RUSH_FRAME_MAX;		// �ړ��ʎZ�o
			_moveVector.GetPX() = mx;					// �ړ��x�N�g���ɑ��
			nextpos = _position.GetX() + mx;	// �ړ���̍��W���Z�o
			if (IsAttackEnd() == true) {			// �}�b�v�`�b�v�ɏՓ˂������H
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
				return;
			}
			// �ړ����X���W���ڕW���W�ɖ����Ȃ��ꍇ�͏������I��
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RUSH_MAX;	// �ڕW���W�̍X�V
			break;
		}
		
		--_actionCount;					// �A�N�V�����J�E���^�����炷
		// �A�N�V�����J�E���^�ɉ����ėP�\���Ԃ�؂�ւ�
		if (_actionCount == 1) _atkInterval = 30;
		else _atkInterval = 20;
		_isAnimation = false;		// �A�j���[�V�������f
		AttackBox(false);				// �U�������؂�
	}

	bool CrowDoll::IsAttackEnd() {
		// �}�b�v�`�b�v�ɏՓ˂��Ă��邩�H
		if (_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction) == mapchip::NORMAL) return true;
		return false;
	}

	void CrowDoll::SetState() {
		if (_isWarp == true) return;	// �]�ڏ���������ꍇ�̓X�L�b�v
		// ��Ԃɉ������������s��
		switch (_crowState) {
			// �ҋ@��Ԃ̏ꍇ
		case CrowState::IDOL:
			// �C���^�[�o���͂��邩�H
			if (_atkInterval == 0) {
				// �{���Ԃ��A�f�o�t�̗��߃J�E���^������ɓ��B���Ă���ꍇ
				if (IsAnger() == IS_ANGER && _debuffChage == DEBUFF_COUNT_MAX) {
					// �m���Ńf�o�t��ԂɑJ�ځi�m��50%�j
					auto number = rand() % 11 + 1;
					if (_debuffChage < number) {
						_debuffChage = 0;		// ���߃J�E���^��������
						ModeChange(CrowState::DEBUFF, enemy::crowdoll::CROW_IDOL);
						_invincible = 120;	// ���G��Ԃɐݒ�
						return;
					}
				}
				auto number = rand() % 3; // �����_���ŏ�ԑJ��
				switch (number) {
				case 0:
					ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_IDOL);				// ��Ԑ؂�ւ�
					GetTarget();				// ���@�̌��ݍ��W���擾����
					WarpOn();						// ���@�̑O�ɒ���
					_actionCount = 4;		// �l�i�K�U��
					_atkInterval = 30;	// �U���P�\����
					break;
				case 1:
					ModeChange(CrowState::GROWARM, enemy::crowdoll::CROW_GROWARM);	// ��Ԑ؂�ւ�
					GetTarget();				// ���@�̍��W�擾
					PlaySe(enemy::crowdoll::SE_GROWARM);
					break;
				case 2:
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);				// ��Ԑ؂�ւ�
					_actionCount = IsAnger();		// �؂�Ă���ꍇ�͏�����ǉ��Ŏ��s����
					WarpOn();										// ���@�̑O�ɒ���
					break;
				}
			}
			break;
			// �A���؂��Ԃ̏ꍇ
		case CrowState::RUSH:
			if (_atkInterval == 0) {		// �҂����Ԃ��Ȃ��ꍇ
				// �A�N�V�����J�E���^������ꍇ
				if (0 < _actionCount) {
					AttackBox(true);				// �U��������I��
					Rash();									// ���b�V���A�N�V�������s
					break;
				}
				// �A�N�V�����J�E���^��0�̏ꍇ
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;		// ������
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
					_atkInterval = 60;
					_isAnimation = true;
					// �{���Ԃ̏ꍇ�A�f�o�t�̗��߃J�E���^�𑝂₷�i����L��j
					if(IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
				}
				break;
			}
			break;
			// �����U��
		case CrowState::BLINK:
			// �����Ă���ꍇ
			if (_stand == true) {
				if (_wait != true) {
					AddSmokeEffect();		// �G�t�F�N�g����
					_wait = true;				// �������~�߂�
					_atkInterval = 60;	// �P�\���Ԑݒ�
					break;
				}
				// �A�j���[�V�����J�E���^������ɓ��B�����ꍇ
				else if (AnimationCountMax() == true) {
					// �҂����Ԃ͏I�����Ă��邩�H
					if (_atkInterval == 0) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
						_atkInterval = 60;
						_wait = false;
						_isAnimation = true;
						if (IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
					}
					// �I�����Ă��Ȃ��ꍇ�A�A�j���[�V�����̍Đ����s��Ȃ�
					else if (_isAnimation == true) _isAnimation = false;
				}
				break;
			}
			break;
			// ���ݏ�Ԃ̏ꍇ
		case CrowState::WINCE:
			// �A�j���[�V�����J�E���^������ɓ��B�����ꍇ
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
				break;
			}
			break;
			// �L�т�r��Ԃ̏ꍇ
		case CrowState::GROWARM:
			// ���[�V�������ő傩�A�r�𖢐����̏ꍇ
			if (IsAnimationMax() == true && _arm == false) {
				// �G�t�F�N�g�̐�������ѓo�^
				auto arm = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ARM, Vector2(_target.GetX(), 655), 24 * 2);
				arm->SetDamageEffect(50, 50, 0, 300, 10);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(arm), effect::type::FORMER);
				_arm = true;	// �r�𐶐�����
				break;
			}
			else if (AnimationCountMax() == true) {		// �J�E���g���}�b�N�ɂȂ����ꍇ�A��Ԃ�؂�ւ���
				_arm = false;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
				if (IsAnger() == IsAnger() && _debuffChage != DEBUFF_COUNT_MAX) ++_debuffChage;
				break;
			}
			break;

		case CrowState::ROAR:		// ���K��Ԃ̏ꍇ
			// �摜�L�[�����K�ł͂Ȃ��ꍇ
			if (_divKey.first != enemy::crowdoll::CROW_ROAR) {
				// y���W�����n�_�܂œ��B��������K���J�n����
				if (_position.IntY() == DEFAULT_Y) {
					_actionCount = 3;		// �l��J��Ԃ�
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_ROAR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
					ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_ROAR);						// ��Ԑ؂�ւ�
					// ���K�G�t�F�N�g�̐�������ѓo�^
					auto roar = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ROAR, _position, 40);
					roar->SetLoop(3);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(roar), effect::type::FORMER);
					_game.GetObjectServer()->GetPlayer()->KnockBack(IsPlayerPos(_moveVector.GetX()));
					DollsEnd();		// ����D�����
					break;
				}
			}
			// �A�j���[�V�����J�E���^������ɓ��B�����ꍇ
			else if (AnimationCountMax() == true) {
				// �C���^�[�o�����Ȃ��ꍇ
				if (_actionCount == 0) {
					// �ҋ@��ԂɑJ�ڂ���
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
					break;
				}
				--_actionCount;
			}
			break;
			// �f�o�t���
		case CrowState::DEBUFF:
			// �摜�L�[���ҋ@��Ԃ̏ꍇ
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				if (_atkInterval == 0 && AnimationCountMax() == true && _isWarp != true) {
					WarpOn();		// ���[�v
					PlaySe(enemy::crowdoll::SE_DEBUF);	// SE��炷
					_atkInterval = 20;
				}
				break;
			}
			// �摜�L�[���Ńf�o�t�̏ꍇ
			if (_divKey.first == enemy::crowdoll::CROW_DEBUFF) {
				// �A�j���[�V�����ԍ�����v���Ă��A�f�o�t�t���O���U�̏ꍇ
				if (AnimationNumber() == DEBUFF_START && _debuff == false) {
					_debuff = true;
					_isAnimation = false;
					_atkInterval = 60;
					return;
					// �f�o�t�t���O���^�̏ꍇ
				} if (_debuff == true) {
					if (_atkInterval == 0 && _isAnimation != true) {
						_isAnimation = true;
						AddDebuffEffect();	// �f�o�t����
						return;
					// �A�j���[�V�����J�E���^������ɓ��B�����ꍇ
					} else if (AnimationCountMax() == true) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
						_debuff = false;	// �f�o�t�t���O���U�ɂ���
						_atkInterval = 60;
					}
				}
				break;
			}
			return;
		}
	}

	bool CrowDoll::IsDead() {
		if (_crowState != CrowState::DEATH) return false;
		return true;
	}

	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_crowState == CrowState::DEATH) return;		// ����ł���ꍇ�͏I��
		if (_invincible != 0) return;									// ���G���Ԃ�����ꍇ�͒��f
		if (IsVital() != true) return;								// �����Ȃ��ꍇ�����f
		// ���@�͒D���A�N�V���������H
		if (ckey == PKEY_ROB) {
			// �����U����(�������[�V������)�͒ʏ�̓����蔻��Ɣ�����s��
			if (_crowState == CrowState::BLINK && _divKey.first == enemy::crowdoll::CROW_BLINK) {
				auto vital = BlinkVitalPart(_mainCollision, CROW_VITAL);		// �}���̎Z�o
				if (vital.first.HitCheck(acollision) == true || vital.second.HitCheck(acollision) == true) {
					_direction = !direction;		// �����𔽓]������
				} else return;
			} else if (_crowState != CrowState::BLINK) {
				auto vitalpart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalpart.HitCheck(acollision) == true) {
				} else return;
			}
			// �e��ϐ���������
			_atkInterval = 0;
			_actionCount = 0;
			_isAnimation = true;
			_wait = false;
			_moveVector = {};
			// ���S�������^�̏ꍇ�͏������I��
			if (DeathOn() == true) return;
			// ���ݏ�Ԃɂ���
			ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);
			auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
			AddSoul();					// ���𐶂ݏo��
			--_life;						// �ϋv�l�����炷
			// �{���Ԃɓ˓������ꍇ�͏������I��
			if(AngerOn() == true) return;
			_invincible = 60;		// ���G���Ԃ̐ݒ�
			return;
		}
	}

	bool CrowDoll::IsVital() {
		// ���݂̏�Ԃ͍���D���邩�H
		switch (_crowState) {
		case CrowState::DEBUFF:
		case CrowState::ROAR:
		case CrowState::WINCE:
		case CrowState::SLEEP:
			return false;
		case CrowState::RUSH:
		case CrowState::BLINK:
		case CrowState::IDOL:
			return true;
		default:
			return false;
		}
	}

	bool CrowDoll::IsActive() {
		// �������������Ă��Ȃ��ꍇ�͏������I��
		if (_setup != true) return false;
		if (_invincible != 0) --_invincible;		// ���G���Ԃ�����ꍇ�͌��炷����
		// ���S��Ԃ܂��͔񊈓���Ԃ̏ꍇ
		if (_crowState == CrowState::DEATH || _crowState == CrowState::SLEEP) {
			if (AnimationCountMax() != true) ObjectBase::AnimationCount();
			return false;
		}
		if (0 < _atkInterval) --_atkInterval;		// �҂����Ԃ����炷
		if (_isAnimation == true )	ObjectBase::AnimationCount();		// �J�E���g�𑝂₷
		return true;
	}

	void CrowDoll::IsBattle() {
		if (_crowState == CrowState::DEATH) return;		// ����ł���ꍇ�͏������I��
		// �{�X�퓬�t���O���I���ɂȂ��Ă��邩�H
		if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return;	
		// ���@��X���W�͖ڕW�n�_�܂œ��B���Ă��邩�H
		if (_game.GetObjectServer()->GetPlayer()->GetPosition().GetX() < _game.GetMapChips()->GetMapSizeWidth() - HALF_WINDOW_W) return;
		_game.GetModeServer()->GetModeMain()->BossBattle();		// ���B���Ă���ꍇ�̓{�X�퓬�����Ăяo��
		return;
	}

	void CrowDoll::AddSoul() {
		auto player = _game.GetObjectServer()->GetPlayer();				// ���@�̏����擾����
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());	// ���𐶐�����
		srand((unsigned int)time(NULL));	// ����������
		// �����_���ō��̏�Ԃ��X�V
		auto type = rand() % 2;
		switch (type) {
		case 0:
			soul->SetParameter(1, 7);		// �Ԃ̍�
			break;
		case 1:
			soul->SetParameter(2, 7);		// �̍�
			break;
		default:
			break;
		}
		// ���@�͍��̏�������ɓ��B���Ă��邩�H
		_game.GetObjectServer()->Add(soul);		// �I�u�W�F�N�g�T�[�o�[�ɓo�^����
		soul->SetSpwan(_position);						// ���g�̒��S���W�Ɏ��̉�������
		// ���@�����̏�������ɓ��B���Ă��邩�H
		if (player->IsSoulMax() == true) {
			soul->OwnerNull();												// ���L�҂͂��Ȃ�
		} else player->SoulCatch(std::move(soul));	// �����ł͂Ȃ��ꍇ�͍��̏��L�����v���C���[�ɏ��n
	}

	bool CrowDoll::IsPlayerPos(double px) {
		auto fix = _position.GetX() - px;
		if (fix < 0) return false;	// �E���ɋ���
		if (0 < fix) return true;		// �����ɋ���
	}

	int CrowDoll::IsAnger() {
		// �{���Ԃɓ˓����Ă��邩�H
		if (CROW_ANGER < _life) return IS_NORMAL;		// �ʏ���
		return IS_ANGER;														// �{����
	}

	void CrowDoll::AddWarpEffect(Vector2 spwan) {
		// ���[�v�G�t�F�N�g�̐�������уG�t�F�N�g�T�[�o�ւ̓o�^
		auto warp = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::BLINK, spwan, 30);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(warp), effect::type::FORMER);
	}

	void CrowDoll::AddRushEffect() {
		// �A���G�t�F�N�g�̐���
		auto rush = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::RUSH, _position, 47 * 3, _direction);
		rush->Set(this, -30, -30);	// �Ǐ]�ݒ�
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(rush), effect::type::FORMER);
	}

	void CrowDoll::AddBlinkEffect() {
		// �����U���G�t�F�N�g�̐���
		auto blink = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK_ATTACK, _position, effect::crow::BLINL_ATTACK_MAX * 4);
		blink->Set(this, 0, -150);	// �Ǐ]�ݒ�
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(blink), effect::type::FORMER);
	}

	void CrowDoll::AddSmokeEffect() {
		// �����n�_�̐ݒ�
		Vector2 addpos = { _position.GetX(), (_position.GetY() + ((_mainCollision.GetHeightMax() / 2) - 75)) };
		// ��(�Ռ��g)�G�t�F�N�g�̐���
		auto smoke = std::make_unique<EffectBase>(_game.GetGame(), effect::enemy::HITDROP, addpos, effect::enemy::HIPDROP_MAX * 2);
		smoke->SetDamageEffect(240, 240, -20, 140, 6, 15);	// �_���[�W����̐ݒ�
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(smoke), effect::type::FORMER);
	}

	void CrowDoll::AddDebuffEffect() {
		_game.GetObjectServer()->GetPlayer()->Debuff();		// ���@�̃f�o�t�����Ăяo��
		// �����n�_�̐ݒ�
		auto world = _game.GetMapChips()->GetWorldPosition();
		Vector2 addpos = { world.GetX(), world.GetY() };
		// �f�o�t�G�t�F�N�g�̐ݒ�
		auto debuff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::DEBUF, addpos, effect::crow::DEBUF_MAX * 3);
		debuff->SetLoop(3);		// �ǉ����[�v�񐔂̐ݒ�
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(debuff), effect::type::FORMER);
	}

	void CrowDoll::AddAngerEffect() {
		// �{��G�t�F�N�g�̐���	
		auto anger = std::make_unique<LoopEffect>(_game.GetGame(), effect::crow::AURA, _position, effect::crow::AURA_MAX * 2);
		anger->SetOwner(this);	// ���L�҂̐ݒ�
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(anger), effect::type::FORMER);
	}

	bool CrowDoll::IsPlayerPosition() {
		// �ڕW���W�̎擾�y�уN�����v
		auto target = _target;
		_game.GetMapChips()->Clamp(target);
		// ���@�͍��E�ǂ��瑤�ɋ���̂�
		if (target.IntX() < HALF_WINDOW_W) return false;			// �����ɂ���
		else if (HALF_WINDOW_W < target.IntX()) return true;	// �E���ɂ���
		// ���S���W�ɂ���ꍇ�̓����_���Ŕ���
		switch (rand() % 2) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
		}
	}

	bool CrowDoll::AngerOn() {
		// ���ɓ{���Ԃ̏ꍇ�͏������I��
		if (_isAnger == true) return false;
		// �ϋv�͔͂����܂Ō����Ă��邩�H
		if (IsAnger() == IS_ANGER) {
			_isAnger = true;		// �{���Ԃɓ˓�����
			ModeChange(CrowState::DEBUFF, enemy::crowdoll::CROW_IDOL);	// �f�o�t��ԂɑJ�ڂ���
			AddAngerEffect();		// �{��G�t�F�N�g����
			_invincible = 120;	// ���G���ԓ˓�
			return true;
		}
		return false;
	}

	AABB CrowDoll::NowCollision(std::string key) {
		// ���S��ԂłȂ��ꍇ�A�ʏ��Ԃ̓����蔻���Ԃ�
		if (_crowState != CrowState::DEATH) return _mainCollision;
		// ����ł���ꍇ�͔����k��Ԃ̓����蔻���Ԃ�
		auto ite = _collisions.find(enemy::crowdoll::CROW_DOWN);
		return ite->second;
	}

	void CrowDoll::WarpOn() {
		// ���Ƀ��[�v�t���O���^�̏ꍇ�͏������I��
		if (_isWarp == true) return;
		_isWarp = true;	
		
		AddWarpEffect(_position);		// ���[�v�G�t�F�N�g�̐���

		double px = 0;
		int sound = 0;
		// ���@�����E�ǂ��瑤�ɋ���̂��擾
		bool direction = IsPlayerPosition();
		// �E�ɋ���ꍇ
		if (direction == true) {
			switch (_crowState) {
				// �A���؂�
			case CrowState::RUSH:
				_direction = enemy::MOVE_LEFT;
				// ���[�v��̍��W��ݒ�
				px = (_game.GetMapChips()->GetWorldPosition().GetX() + 700 + _mainCollision.GetWidthMax());
				_warpPos = { px, 820 };
				_actionEnd.GetPX() = px - RUSH_MAX;
				break;
			default:
				break;
			}
		// ���ɋ���ꍇ
		} else {	
			switch (_crowState) {
				// �A���؂�
			case CrowState::RUSH:
				_direction = enemy::MOVE_RIGHT;
				// ���[�v��̍��W��ݒ�
				px = (_game.GetMapChips()->GetWorldPosition().GetX() - 700 - _mainCollision.GetWidthMin());
				_warpPos = { px, 820 };
				_actionEnd.GetPX() = px + RUSH_MAX;
				break;
			default:
				break;
			}
		}
		switch (_crowState) {
			// �f�o�t
		case inr::CrowDoll::CrowState::DEBUFF:
			//�@���[�v��̍��W����ʒ����ɐݒ�
			_warpPos = { static_cast<double>(_game.GetMapChips()->GetMapSizeWidth()) - HALF_WINDOW_W, HALF_WINDOW_H };
			_moveVector = { 0, 0 };
			break;
			// �����U��
		case inr::CrowDoll::CrowState::BLINK:
			PlaySe(enemy::crowdoll::SE_BLINK_ATTACK);
			GetTarget();																// ���@�̌��ݍ��W�擾
			_warpPos = { _target.GetX(), DEFAULT_Y };		// ���W�ύX
			_moveVector = { 0, 0 };
			break;
			// �ҋ@
		case inr::CrowDoll::CrowState::IDOL:
			_warpPos = _game.GetMapChips()->GetWorldPosition();
			_moveVector = { 0, 0 };
			break;
		default:
			break;
		}
		_atkInterval = 15;	// �P�\���Ԃ̐ݒ�
	}

	std::pair<AABB, AABB> CrowDoll::BlinkVitalPart(Collision& col, int vital) {
		// ���E�����ɔ���𐶐�����
		AABB left = { {col.GetMax().GetX() - vital, col.GetMin().GetY()}, {col.GetMax().GetX(), col.GetMax().GetY()}, true};
		AABB right = { {col.GetMax().GetX() + vital, col.GetMin().GetY()}, {col.GetMax().GetX(), col.GetMax().GetY()}, true};
		return std::make_pair(left, right);
	}

	bool CrowDoll::AttackBox(bool flag) {
		// �A���؂�̓����蔻����擾
		auto rush = _collisions.find(enemy::crowdoll::CROW_RUSH);
		// �����蔻�肪�U�̏ꍇ�͏������I��
		if (rush->second.GetCollisionFlag() == flag) return false;
		// �e��t���O�̐؂�ւ�
		rush->second.SetCollisionFlag() = flag;
#ifdef _DEBUG
		rush->second.SetDrawFlag() = flag;
#endif
		return true;
	}

	bool CrowDoll::DeathOn() {
		// �ϋv�l������ꍇ�͏������I��
		if (_life != 0) return false;
		_game.GetModeServer()->GetModeMain()->BossEnd();						// �{�X����I������
		ModeChange(CrowState::DEATH, enemy::crowdoll::CROW_DOWN);		// ���S��ԂɈڍs����
		// ���݂̒n�_�Ɏ��S�G�t�F�N�g�𔭐�������
		auto death = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::DEATH, _position, effect::crow::DEATH_MAX * 3);
		PlaySe(enemy::crowdoll::SE_DEATH);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(death), effect::type::FORMER);
		return true;
	}

	void CrowDoll::DollsEnd() {
		// �G���擾
		auto enemys = _game.GetObjectServer()->GetEnemys();
		for (auto ite : enemys) {
			// ����ۗL����A�N���E�h�[���ȊO�̓G�����S������
			if (ite->GetEnemyType() == EnemyType::CROW_DOLL) continue;
			if (ite->IsEmpty() == true) continue;
			ite->SoulPop();
		}
	}
}
