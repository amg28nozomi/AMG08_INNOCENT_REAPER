#include "CrowDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Player.h"
#include "EffectServer.h"
#include "EffectBase.h"
#include "TrackingEffect.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "MapChips.h"
#include "SoulSkin.h"

#include <random>
#include <time.h>
#include <memory>

namespace {
	constexpr auto CROW_WIDTH = 60;	// ���̓����蔻��
	constexpr auto CROW_HEIGHT = 260;	// �c�̓����蔻��

	constexpr auto CROW_VITAL = 20;
	constexpr auto CROW_LIFE_MAX = 10;
	constexpr auto CROW_ANGER = 5;	// �{���ԂɂȂ邩

	constexpr auto FLAOT_MAX = 200;
	constexpr auto DEFAULT_Y = 400; // 870 - 300;

	constexpr auto IS_ANGER = 1;
	constexpr auto IS_NORMAL = 0;

	constexpr auto RASH_MAX = 250;

	constexpr auto FLOAT_MAX = 540;
}

namespace inr {

	CrowDoll::CrowDoll(Game& game) : EnemyBase(game), _cState(CrowState::SLEEP) {
		_type = ObjectBase::ObjectType::ENEMY;
		_aState = ActionState::IDOL;
		_eType = EnemyType::CROW_DOLL;
		_cState = CrowState::SLEEP;
		Init();
	}

	void CrowDoll::Init() {
		_actionEnd = { 0, 0 };
		_target = { 0, 0 };
		_divKey = { enemy::crowdoll::CROW_DOWN, "" };
		_mainCollision = { _position, CROW_WIDTH / 2, CROW_HEIGHT / 2, false };	// �����蔻��
		_collisions = {
			{enemy::crowdoll::CROW_RUSH, {_position, 210, -30, CROW_HEIGHT / 2 , CROW_HEIGHT / 2, true}},	// �A���U���̓����蔻��
			{enemy::crowdoll::CROW_DOWN, {_position, 50, 50, 50, 90, true}},
		};

		// { �摜�����p�L�[, {���t���[����, SE�̍Đ�����} }
		_motionKey = {
			{enemy::crowdoll::CROW_IDOL, {enemy::crowdoll::motion::IDOL * 2, 0}},
			{enemy::crowdoll::CROW_RUSH , {enemy::crowdoll::motion::RUSH  * 2, 20}},
			{enemy::crowdoll::CROW_BLINK , {enemy::crowdoll::motion::BLINK * 2, 20}},
			{enemy::crowdoll::CROW_GROWARM , {enemy::crowdoll::motion::GROWARM * 3, 20}},
			{enemy::crowdoll::CROW_ROAR , {enemy::crowdoll::motion::ROAR * 4, 50}},
			{enemy::crowdoll::CROW_DEBUF, {enemy::crowdoll::motion::DEBUF * 3, 50}},
			{enemy::crowdoll::CROW_DOWN , {enemy::crowdoll::motion::DOWN * 3, 50}},
			{enemy::crowdoll::CROW_WINCE, {enemy::crowdoll::motion::WINCE * 3, 50}},
		};
		_aCount = GetSize(_divKey.first) - 1;
		_atkInterval = 0;
		_actionCount = 0;
		_muteki = 0;
		_life = 10;
		_arm = false;
		_setup = false;
		_changeGraph = false;
		_wait = false;
		_direction = true;
		_changeState = false;
		_isAnimation = false;
		_isWarp = false;
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
		if (_isWarp == true && _atkInterval == 0) Warp();
		SetState();
		Floating();
		Attack();	// �_���[�W����
		Move();
	}

	void  CrowDoll::Draw() {

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
			ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
			_isAnimation = true;
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

		for (auto&& it : _collisions) it.second.Update(_position, _direction);
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
		
		if (_cState != CrowState::RUSH) return;	// �U���{�b�N�X���Ȃ��ꍇ�͏������s��Ȃ�
		if (_atkInterval != 0) return;	// �҂����Ԓ��͔�����s��Ȃ�
		auto damageBox = _collisions.find(_divKey.first);	// �{�b�N�X�͂��邩�H
		if (damageBox == _collisions.end()) return;
		if(damageBox->second.HitCheck(playerBox) == true) player->Damage(IsPlayerPos(player->GetPosition().GetX()));	// ���W�����ɔ�΂�
	}

	bool CrowDoll::IsGravity() {
		switch (_cState) {
		case CrowState::DEATH:
		case CrowState::SLEEP:
		case CrowState::BLINK:
		case CrowState::RUSH:
			if (_divKey.first == enemy::crowdoll::CROW_IDOL) {
				_gravity = 0;
				_stand = false;
				return true;
			}
			_gravity += FRAME_G;	// �����x�����Z
			if (MAX_G < _gravity) _gravity = MAX_G;
			if (IsStandChip()) {
				if (0 < _gravity) _stand = true;
				if (_cState == CrowState::BLINK) {
					_gravity = 0;
					// WarpOn();	// ���[�v�ړ�
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
		if (_cState == CrowState::IDOL || _cState == CrowState::ROAR) {
			if (_position.GetY() <= DEFAULT_Y) return false;
			_gravity -= 0.25;
			if (_position.GetY() + _gravity < DEFAULT_Y) {
				_position.GetPY() = DEFAULT_Y;
				_gravity = 0;
			}
			return true;
		}
		return false;
	}

	void CrowDoll::Warp() {
		_isWarp = false;
		_isAnimation = true;	// �A�j���[�V�����ĊJ
		AddWarpEffect(_warpPos, true);	// �Ǐ]�����L��
		_position = _warpPos;

		switch (_cState) {
		case CrowState::RUSH:
			PlaySe(enemy::crowdoll::SE_RUSH);
			ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_RUSH);
			AddRushEffect();
			return;
		case CrowDoll::CrowState::DEBUF:
			return;
		case CrowDoll::CrowState::ROAR:
			return;
		case CrowDoll::CrowState::BLINK:
			ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_BLINK);	// ��Ԑ؂�ւ�
			AddBlinkEffect();
			return;
		case inr::CrowDoll::CrowState::IDOL:
			return;
		default:
			return;
		}
	}

	void CrowDoll::Rash() {
		_isAnimation = true;
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
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
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
				StopSoundMem(se::SoundServer::GetSound(enemy::crowdoll::SE_RUSH));
				return;
			}
			if (nextpos < _actionEnd.GetX()) return;
			_actionEnd.GetPX() = nextpos + RASH_MAX;
			break;
		}
		
		--_actionCount;
		if (_actionCount == 1) _atkInterval = 30;
		else _atkInterval = 20;
		_isAnimation = false;
		AttackBox(false);
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
		return false;
	}

	bool CrowDoll::SetState() {
		if (_isWarp == true) return false;	// �]�ڏ���������ꍇ�̓X�L�b�v
		// ��Ԃɉ������������s��
		switch (_cState) {
		case CrowState::IDOL:	// �󒆑ҋ@�̏ꍇ
			// �C���^�[�o�������Ɏ��̃A�N�V���������s����
			if (_atkInterval == 0) {
				auto number = rand() % 3; // %3
				switch (number) {
				case 0:
					ModeChange(CrowState::RUSH, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
					GetTarget();	// ���@�̌��ݍ��W���擾����
					WarpOn();	// ���@�̑O�ɒ���
					_actionCount = 4;
					_atkInterval = 30;
					break;
				case 1:
					ModeChange(CrowState::GROWARM, enemy::crowdoll::CROW_GROWARM);	// ��Ԑ؂�ւ�
					GetTarget();
					PlaySe(enemy::crowdoll::SE_GROWARM);
					break;
				case 2:
					ModeChange(CrowState::BLINK, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
					_actionCount = IsAnger();	// �؂�Ă���ꍇ�͏�����ǉ��Ŏ��s����
					WarpOn();	// ���@�̑O�ɒ���
					// _atkInterval = 60;
					break;
				}
			}
			break;
		case CrowState::RUSH:
			if (_atkInterval == 0) {	// �҂����Ԃ��Ȃ��ꍇ
				if (0 < _actionCount) {
					AttackBox(true);
					Rash();	// ���b�V���A�N�V�������s
					break;
				}
				else if (_actionCount == 0) {
					_actionEnd.GetPX() = 0;
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
					_atkInterval = 60;
					_isAnimation = true;
				}
				// ���̏�ԂɑJ�ڂ���
				break;
			}
			break;
		case CrowState::BLINK:
			// �����Ă���ꍇ
			if (_stand == true) {
				if (_wait != true) {
					_wait = true;
					_atkInterval = 60;
					break;
				}
				else if (AnimationCountMax() == true) {
					// �҂����Ԃ͏I�����Ă��邩�H
					if (_atkInterval == 0) {
						ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
						_atkInterval = 60;
						_wait = false;
						_isAnimation = true;
					} else if (_isAnimation == true) _isAnimation = false;
				}
				break;
			}
			break;
		case CrowState::WINCE:
			if (IsAnimationMax() == true) {
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
				break;
			}
			break;
		case CrowState::GROWARM:
			// �r��}�����u�ԂɃG�t�F�N�g�𔭐�������
			if (IsAnimationMax() == true && _arm == false) {	// ���[�V�������ő傩�A�r���������̏ꍇ�̂ݘr�𐶐�����
				auto effarm = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ARM, Vector2(_target.GetX(), 655), 24 * 2);	// �G�t�F�N�g���쐬(950)
				effarm->SetDamageEffect(50, 50, 0, 300,  10);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(effarm), effect::type::FORMER);
				_arm = true;
				break;
			}
			else if (AnimationCountMax() == true) {	// �J�E���g���}�b�N�ɂȂ����ꍇ�A��Ԃ�؂�ւ���
				_arm = false;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);	// ��Ԑ؂�ւ�
				_atkInterval = 60;
				break;
			}
			break;

		case CrowState::ROAR:	// ���K����
			if (_divKey.first != enemy::crowdoll::CROW_ROAR) {
				// y���W�����n�_�܂œ��B��������K���J�n����
				if (_position.IntY() == DEFAULT_Y) {
					_actionCount = 3;	// �l��J��Ԃ�
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_ROAR);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
					ModeChange(CrowState::ROAR, enemy::crowdoll::CROW_ROAR);	// ��Ԑ؂�ւ�
					auto roar_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::ROAR, _position, 40);// _game.GetMapChips()->GetWorldPosition(), 30);
					roar_eff->SetLoop(3);
					_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(roar_eff), effect::type::FORMER);
					break;
				}
			} else if (AnimationCountMax() == true) {
				if (_actionCount == 0) {
					ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
					break;
				}
				--_actionCount;
			}
			break;
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
					// �e��l������
					_atkInterval = 0;
					_actionCount = 0;
					_isAnimation = true;
					_wait = false;
					_moveVector = {};

					// ����D����
					ModeChange(CrowState::WINCE, enemy::crowdoll::CROW_WINCE);	// ���ݏ�Ԃɂ���
					auto sound = se::SoundServer::GetSound(enemy::crowdoll::SE_VOICE);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));	// ������炷
					AddSoul();	// ���𐶂ݏo��
					--_life;
					if(_life == 0) ModeChange(CrowState::SLEEP, enemy::crowdoll::CROW_DOWN);	// ���S����
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
		case CrowState::RUSH:
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
		if (_isAnimation == true )ObjectBase::AnimationCount();	// �J�E���g�𑝂₷
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
		// ���@�͍��̏�������ɓ��B���Ă��邩�H
		_game.GetObjectServer()->Add(soul);	// �I�u�W�F�N�g�T�[�o�[�ɓo�^����
		soul->SetSpwan(_position);	// ���g�̒��S���W�Ɏ��̉�������
		if (player->IsSoulMax() == true) {
			soul->OwnerNull();	// ���L�҂͂��Ȃ�
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

	bool CrowDoll::AddWarpEffect(Vector2 spwan, bool target) {
		if (target == true) {
			auto warp_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK, spwan, 24 * 2);
			warp_eff->Set(this);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(warp_eff), effect::type::FORMER);
			return true;
		} else {
			auto warp_eff = std::make_unique<EffectBase>(_game.GetGame(), effect::crow::BLINK, spwan, 30);
			_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(warp_eff), effect::type::FORMER);
			return true;
		}
	}

	bool CrowDoll::AddRushEffect() {
		bool eff_dir = _direction == enemy::MOVE_LEFT;

		auto rush_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::RUSH, _position, 47 * 3, _direction);
		rush_eff->Set(this, -30, -30);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(rush_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::AddBlinkEffect() {
		auto blink_eff = std::make_unique<TrackingEffect>(_game.GetGame(), effect::crow::BLINK_ATTACK, _position, effect::crow::BLINL_ATTACK_MAX * 3);
		blink_eff->Set(this, 0, -150);
		_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(blink_eff), effect::type::FORMER);
		return true;
	}

	bool CrowDoll::IsPlayerPosition() {
		auto player_p = _target;
		_game.GetMapChips()->Clamp(player_p);
		// ���@�͍��E�ǂ��瑤�ɋ���̂�
		if (player_p.IntX() < HALF_WINDOW_W) return false;	// �����ɂ���
		else if (HALF_WINDOW_W < player_p.IntX()) return true;	// �E���ɂ���
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

	AABB CrowDoll::NowCollision(std::string key) {

		return _mainCollision;
	}

	void CrowDoll::WarpOn() {
		if (_isWarp == true) return;
		_isWarp = true;	// �]��
		
		AddWarpEffect(_position);	// �]�ڃG�t�F�N�g�̐���
		// SE��炷

		double px = 0;
		int sound = 0;

		bool isPlayer = IsPlayerPosition();

		if (isPlayer == true) {	// ���@�͍��ɋ���
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_LEFT;
				px = (_game.GetMapChips()->GetWorldPosition().GetX() + 700 + _mainCollision.GetWidthMax());
				_warpPos = { px, 820 };	// �]�ڍ��W�ɑ������
				_actionEnd.GetPX() = px - RASH_MAX;
				break;
			default:
				break;
			}
		}
		else {
			switch (_cState) {
			case CrowState::RUSH:
				_direction = enemy::MOVE_RIGHT;
				px = (_game.GetMapChips()->GetWorldPosition().GetX() - 700 - _mainCollision.GetWidthMin());
				_warpPos = { px, 820 };
				_actionEnd.GetPX() = px + RASH_MAX;
				break;
			default:
				break;
			}
		}
		switch (_cState) {
		case inr::CrowDoll::CrowState::DEBUF:
			//���S���W�Ƀ��[�v����
			_warpPos = { static_cast<double>(_game.GetMapChips()->GetMapSizeWidth()) - HALF_WINDOW_W, HALF_WINDOW_H };
			_moveVector = { 0, 0 };
			break;
		case inr::CrowDoll::CrowState::ROAR:
			/*if (AnimationCountMax() == true) {
				_atkInterval = 30;
				ModeChange(CrowState::IDOL, enemy::crowdoll::CROW_IDOL);
				Warp();
			}*/
			break;
		case inr::CrowDoll::CrowState::BLINK:
			// ���@�̓���Ƀ��[�v����
			GetTarget();
			_warpPos = { _target.GetX(), DEFAULT_Y };	// ���W�ύX
			_moveVector = { 0, 0 };	// �ړ��ʂ͏���
			break;
		case inr::CrowDoll::CrowState::IDOL:
			_warpPos = _game.GetMapChips()->GetWorldPosition();
			_moveVector = { 0, 0 };
			break;
		default:
			break;
		}

		_atkInterval = 15;	// 10�t���[����ɃA�N�V���������s����
	}

	bool CrowDoll::AttackBox(bool flag) {
		auto rush = _collisions.find(enemy::crowdoll::CROW_RUSH);
		if (rush->second.GetCollisionFlg() == flag) return false;
		rush->second.GetCollisionFlgB() = flag;
#ifdef _DEBUG
		rush->second.GetbDrawFlg() = flag;
#endif
		return true;
	}
}
