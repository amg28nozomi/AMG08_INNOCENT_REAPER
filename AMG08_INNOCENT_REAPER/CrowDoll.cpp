#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "MapChips.h"
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

	constexpr auto FLAOT_MAX = 200;
	constexpr auto DEFAULT_Y = 870 - 300;

	constexpr auto IS_ANGER = 1;
	constexpr auto IS_NORMAL = 0;

	constexpr auto RASH_MAX = 150;
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;
		_aState = ActionState::IDOL;
		_eType = EnemyType::CROW_DOLL;
		Init();
	}

	void CrowDoll::Init() {
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// �����蔻��
		_collisions = {
			{enemy::crowdoll::CROW_RASH, {_position, 0, 120, 130 , CROW_HEIGHT / 2, true}},	// �A���U���̓����蔻��
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {25, 0}},
			{enemy::crowdoll::CROW_RASH , {40, 20}},
			{enemy::crowdoll::CROW_BLINK , {25, 20}},
			{enemy::crowdoll::CROW_GROWARM , {25, 20}},
			{enemy::crowdoll::CROW_ROAR , {25, 50}},
			{enemy::crowdoll::CROW_DEBUF, {30, 50}},
			{enemy::crowdoll::CROW_DOWN , {25, 50}},
			{enemy::crowdoll::CROW_WINCE, {25, 50}},
		};
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_actionCount = 0;
		_muteki = 0;
		_setup = false;
		_changeGraph = false;
	}

	void CrowDoll::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions().at(0);	// ���W���X�V
		Init();
	}

	// �U���p�^�[���́u���b�V�����������󒆑ҋ@���r�h�����ҋ@���󒆑ҋ@�v

	void CrowDoll::Process() {
		IsGravity();
		// �o�g���J�n�O�̏���
		IsBattle();
		WakeUp();	// �N���オ��
		if (IsActive() != true) return;	// ������ԂłȂ��ꍇ�͏������s��Ȃ�
		SetState();
		Floating();
		Attack();	// �_���[�W����
		Move();
	}

	void CrowDoll::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
#endif
	}

	void CrowDoll::WakeUp() {
		if (_game.GetModeServer()->GetModeMain()->BossFlag() != true) return;
		if (_setup == true) return;
		--_aCount;	// �����オ�点��
		if (_aCount == 0) {
			_setup = true;	// �Z�b�g�A�b�v����
			_mainCollision.GetCollisionFlgB() = true;	// �����蔻����I���ɂ���
			auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
			ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
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

	void CrowDoll::ModeChange(CrowState nextState, std::string key) {
		_divKey.first = key;
		_cState = nextState;
		_changeGraph = true;
	}

	void CrowDoll::Move() {
		ChangeDirection();	// �����̒���
		_moveVector.GetPY() = _gravity;
		_game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction);	// �����o������

		_position = _position + _moveVector;
		_mainCollision.Update(_position, _direction);

		for (auto it : _collisions) it.second.Update(_position, _direction);
		_moveVector = { 0, 0 };
	}

	void CrowDoll::Attack() {
		// �U������
		auto player = _game.GetObjectServer()->GetPlayer();	// ���@
		auto playerBox = player->GetMainCollision();	// �v���C���[�̓����蔻��
		
		// �܂��͓G�̓����蔻��ƐڐG������s��
		if (_mainCollision.HitCheck(playerBox) == true) {
			// �����������H
			player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// ���W�����ɔ�΂�
			return;	// �U�����肪�������ꍇ�͏������I������
		}
		
		auto damageBox = _collisions.find(_divKey.first);	// �{�b�N�X�͂��邩�H
		if (damageBox == _collisions.end()) return;
		if(damageBox->second.HitCheck(playerBox) == true) player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// ���W�����ɔ�΂�
	}

	bool CrowDoll::IsGravity() {
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::RASH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
			_gravity += FRAME_G;	// �����x�����Z
			if (MAX_G < _gravity) _gravity = MAX_G;
			if (IsStandChip()) {
				if (0 < _gravity) _stand = true;
				if (_cState == CrowState::BLINK) {
					_atkInterval = 30;
					_gravity = 0;
					Warp();	// ���[�v�ړ�
					return true;
				}	// ���[�v�������Ăяo��
				_gravity = 0;
			} else {
				_stand = false;
			}

			break;
		default:
			if (0 < _gravity) _gravity = 0;
			return false;	// �d�͏����͍s��Ȃ�
		}
	}

	bool CrowDoll::Floating() {
		if (_cState != CrowState::IDOL) return false;
		if (_position.GetY() <= DEFAULT_Y) return false;
		_gravity -= 0.25;
		if (_position.GetY() + _gravity < DEFAULT_Y) { 
			_position.GetPY() = DEFAULT_Y;
			_gravity = 0;
		}
		return true;
	}

	void CrowDoll::Warp() {
		auto p = _position.GetX() - _target.GetX();	// ���@�͂ǂ��瑤�ɂ��邩�H
		double px = 0;
		int sound = 0;
		if (p < 0) {	// ���@�͍��ɋ���
			switch (_cState) {
			case CrowState::RASH:
				_direction = enemy::MOVE_RIGHT;
				px = (_target.GetX() - (_mainCollision.GetWidthMin()) * 5);
				_position = { px, 870 };
				_actionEnd.GetPX() = px - RASH_MAX;
				sound = se::SoundServer::GetSound(enemy::crowdoll::SE_RASH);
				PlaySoundMem(sound, se::SoundServer::GetPlayType(enemy::crowdoll::SE_RASH));
				break;
			default:
				break;
			}
		}// ���@�͉E���ɂ���
		else if (0 < p) { 
			switch (_cState) {
			case CrowState::RASH:
				_direction = enemy::MOVE_LEFT;
				px = (_target.GetX() + (_mainCollision.GetWidthMax()) * 5);
				_position = { px, 870 };
				_actionEnd.GetPX() = px + RASH_MAX;
				sound = se::SoundServer::GetSound(enemy::crowdoll::SE_RASH);
				PlaySoundMem(sound, se::SoundServer::GetPlayType(enemy::crowdoll::SE_RASH));
				break;
			default:
				break;
			}
		}// ���@�͍����ɂ���
		switch (_cState) {
		case inr::CrowDoll::CrowState::DEBUF:
			//���S���W�Ƀ��[�v����
			_position = { static_cast<double>(_game.GetMapChips()->GetMapSizeWidth()) - HALF_WINDOW_W, HALF_WINDOW_H};
			_moveVector = { 0, 0 };
			break;
		case inr::CrowDoll::CrowState::ROAR:
			break;
		case inr::CrowDoll::CrowState::BLINK:
			// ���@�̓���Ƀ��[�v����
			GetTarget();
			_position = { _target.GetX(), _target.GetY() - 600 };	// ���W�ύX
			_moveVector = { 0, 0 };	// �ړ��ʂ͏���
			break;
		default:
			break;
		}

		_atkInterval = 10;	// 10�t���[����ɃA�N�V���������s����
	}

	void CrowDoll::Rash() {
		// �A���U��
		double mx = 0;
		double nextpos = 0;
		switch (_direction) {
		case enemy::MOVE_LEFT:
			mx = -RASH_MAX / 15;
			_moveVector.GetPX() = mx;
			nextpos = _position.GetX() + mx;
			if (IsAttackEnd() == true) {
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RASH));
				return;
			}
			if (_actionEnd.GetX() < nextpos) return;
			_actionEnd.GetPX() = nextpos - RASH_MAX;
			break;
		case enemy::MOVE_RIGHT:
			mx = RASH_MAX / 15;
			_moveVector.GetPX() = mx;
			nextpos = _position.GetX() + mx;
			if (IsAttackEnd() == true) {
				--_actionCount;
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RASH));
				return;
			}
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RASH_MAX;
			break;
		}
		--_actionCount;
		_atkInterval = 20;
	}

	void CrowDoll::Blink() {
		_gravity += 0.5;	// �d�͉���
	}

	void CrowDoll::Debuf() {
		// ���@�ɑ΂��Ď�̌��ʂ�t�^����
		_game.GetObjectServer()->GetPlayer()->Debuf();
		// �f�o�t�G�t�F�N�g�̐���
		// auto debufEff = std::make_unique<EffectBase>(_game.GetGame(), )
	}

	bool CrowDoll::IsAttackEnd() {
		if (_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector, _direction) == mapchip::NORMAL) return true;
		auto moveout = _position + _moveVector;
		return false;
	}

	bool CrowDoll::SetState() {
		// ��Ԃɉ������������s��
		switch (_cState) {
		case CrowState::IDOL:	// �󒆑ҋ@�̏ꍇ
			// �C���^�[�������Ɏ��̃A�N�V���������s����
			if (_atkInterval == 0) {
				ModeChange(CrowState::RASH, enemy::crowdoll::CROW_RASH);	// ��Ԑ؂�ւ�
				GetTarget();	// ���@�̌��ݍ��W���擾����
				Warp();	// ���@�̑O�ɒ���
				_actionCount = 4;
			}
			break;
		case CrowState::RASH:
			if (_atkInterval == 0) {
				if (0 < _actionCount) {
					Rash();	// ���b�V���A�N�V�������s
					break;
				}
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
					_actionCount = IsAnger();	// �؂�Ă���ꍇ�͏�����ǉ��Ŏ��s����
					_atkInterval = 60;
				}
				// ���̏�ԂɑJ�ڂ���
				break;
			} 
		case CrowState::BLINK:
			if (_atkInterval == 0) {
				if (0 != _actionCount) {
					Blink();	// ���b�V���A�N�V�������s
					break;
				}
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
				break;
			}
		case CrowState::WINCE:
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
			}
		}
		return true;
	}

	void CrowDoll::CollisionHit(const std::string ckey, Collision acollision, bool direction) {
		if (_cState == CrowState::DEATH) return;	// ����ł���ꍇ�͍���D���Ȃ�
		if (_muteki != 0) return;	// ���G���Ԃ�����ꍇ���D���Ȃ�
		if (IsVital() != true) return;	// �����Ȃ��ꍇ������D���Ȃ�
		if (ckey == PKEY_ROB) {	// ����D���ɂ��Ă��邩�H
			auto vitalPart = VitalPart(_mainCollision, CROW_VITAL);
				if (_direction == direction && vitalPart.HitCheck(acollision)) {
					// ����D����
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// ���ݏ�Ԃɂ���
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
					AddSoul();	// ���𐶂ݏo��
					// �����Ŏ��S�������s����������s��
					_muteki = 60;	// ��莞�Ԃ̊ԁA���G��Ԃɂ���
					return;
			}
		}
	}

	bool CrowDoll::IsVital() {
		switch (_cState) {
		case CrowState::DEBUF:
		case CrowState::ROAR:
		case CrowState::WINCE:
		case CrowState::SLEEP:
			return false;
		case CrowState::RASH:
		case CrowState::BLINK:
		case CrowState::IDOL:
			return true;
		default:
			return false;
		}
	}

	bool CrowDoll::IsActive() {
		if (_setup != true) return false;
		if (_muteki != 0) --_muteki;	// ���G���Ԃ�����ꍇ�͌��炷����
		if (_cState == CrowState::DEATH || _cState == CrowState::SLEEP) {
			if (IsAnimationMax() != true) ObjectBase::AnimationCount();
			return false;
		}
		// �҂����Ԃ̏ꍇ��
		if (0 < _atkInterval) --_atkInterval;	// �҂����Ԃ����炷
		else ObjectBase::AnimationCount();	// �J�E���g�𑝂₷
		return true;
	}

	bool CrowDoll::IsBattle() {
		if (_game.GetModeServer()->GetModeMain()->BossFlag() == true) return false;
		if (_game.GetObjectServer()->GetPlayer()->GetPosition().GetX() < _game.GetMapChips()->GetMapSizeWidth() - HALF_WINDOW_W) return false;
		_game.GetModeServer()->GetModeMain()->BossBattle();	// �{�X�o�g�����J�n����
	}

	void CrowDoll::AddSoul() {
		auto player = _game.GetObjectServer()->GetPlayer();	// ���@�̏����擾����
		auto soul = std::make_shared<SoulSkin>(_game.GetGame());	// ���𐶐�����
		srand((unsigned int)time(NULL));	// ����������
		auto type = rand() % 3 + 1;
		switch (type) {
		case 1:
			soul->SetParameter(1, 7);	// �Ԃ̍�
			break;
		case 2:
			soul->SetParameter(2, 7);	// �̍�
			break;
		default:
			break;
		}
		// ���@�͍��̏�������ɓ��B���Ă��邩�H
		if (player->IsSoulMax()) {
			_soul->OwnerNull();	// ���L�҂͂��Ȃ�
			soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������
			_game.GetObjectServer()->Add(std::move(soul));	// �I�u�W�F�N�g�T�[�o�[�ɓo�^����
			return;
		} else player->SoulCatch(std::move(soul));	// �����ł͂Ȃ��ꍇ�͍��̏��L�����v���C���[�ɏ��n
	}

	bool CrowDoll::IsPlayerPos(double px) {
		auto fix = _position.GetX() - px;
		if (fix < 0) return false;
		if (0 < fix) return true;
	}

	int CrowDoll::IsAnger() {
		// �{���Ԃɓ˓����Ă��邩�H
		if (CROW_ANGER < _life) return IS_NORMAL;	// �{���ԂɂȂ��Ă��Ȃ�
		return IS_ANGER;	// �{���Ă���
	}

	AABB CrowDoll::NowCollision(std::string key) {

		return _mainCollision;
	}
}
