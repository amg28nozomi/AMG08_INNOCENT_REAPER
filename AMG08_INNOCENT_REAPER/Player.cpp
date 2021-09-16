#include "Player.h"
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "EnemyBase.h"
#include "SoundServer.h"
#include "SoulSkin.h"
#include "MapChips.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "GimmickBase.h"
#include "Crystal.h"
#include "EffectBase.h"
#include "EffectServer.h"
#include <DxLib.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace {
#ifdef _RELEASE
	constexpr auto START_POSITION_X = inr::WINDOW_H / 2;
	constexpr auto START_POSITION_Y = inr::WINDOW_W / 2;
#endif
#ifdef _DEBUG
	constexpr auto START_POSITION_X = 200;
	constexpr auto START_POSITION_Y = 1900;
#endif

	// constexpr auto ROB

	// constexpr auto PLAYER_HIGHT = 250; // 300
	// constexpr auto PLAYER_WIDTH = 160; // 400

	constexpr auto MAX_SPPED = 6;

	constexpr auto BF_HEIGHT_MIN = 10;	// �{�b�N�X���W�C���p
	constexpr auto BF_HEIGHT_MAX = 20;
	constexpr auto BF_WIDTH = 10;

	// �A�N�V�����̃t���[����
	constexpr auto ACTION_MAX = 3;

	// SE�̃t���[����
	constexpr auto SE_RUN1 = 50;	// �ړ�SE�̃t���[���� 


	// �D���A�N�V�����̓����蔻��
	constexpr auto ROB_WIDTH1 = 25 + 50; //-75;
	constexpr auto ROB_WIDTH2 = -25;
	constexpr auto ROB_HEIGHT1 = 0;
	constexpr auto ROB_HEIGHT2 = 40;
	// �D���A�N�V�����̔���t���[����
	constexpr auto JUDGE_MIN = -1;
	constexpr auto ROB_JUDGEMENT = 8 * 4;// judgement

	// �^����A�N�V�����̓����蔻��
	constexpr auto GIVE_WIDTH1 = 60;
	constexpr auto GIVE_WIDTH2 = -10;
	constexpr auto GIVE_HEIGHT1 = -15;
	constexpr auto GIVE_HEIGHT2 = 55;
	// �^����A�N�V�����̔���t���[����
	constexpr auto GIVE_JUDGEMENT = 10 * 4;

	// �W�����v�A�N�V����
	constexpr auto JUMP_VECTOR = 2;	// �W�����v�̈ړ��x�N�g��
	constexpr auto JUMP_MAX = 16; // 15
	constexpr auto JUMP_Y = 5;

	// �_�b�V���A�N�V�����֘A
	constexpr auto DASH_INTERVAL = 60;	// �_�b�V�����[�V������̃C���^�[�o������
	constexpr auto DASH_TIME = 50 / 4;	// �_�b�V���A�N�V��������������܂ł̎���
	constexpr auto DASH_MAX = 280;	// �_�b�V�����[�V�����̍ő�ړ�����
	constexpr auto DASH_VEC = DASH_MAX / DASH_TIME;	// �ړ��x�N�g��

	constexpr auto DASH_WIDTH1 = 40;
	constexpr auto DASH_WIDTH2 = 30;
	constexpr auto DASH_HEIGHT1 = 10;
	constexpr auto DASH_HEIGHT2 = 70;

	// �m�b�N�o�b�N�֘A
	constexpr auto HIT_MAX = 300; // �ő�ړ���
	constexpr auto HIT_FRAME = 60;	// �m�b�N�o�b�N����
	constexpr auto INVINCIBLE_TIME = 120;	// ���G�t���[��

	// �����o������


	// �e�탂�[�V�����̉摜��
	constexpr auto PF_IDOL = 13;
	constexpr auto PF_RUN = 25;
	constexpr auto PF_DASH = 13;
	constexpr auto PF_JUMP = 6;
	constexpr auto PF_FALL = 10;
	constexpr auto PF_ROB = 13;
	constexpr auto PF_GIVE = 16;
	constexpr auto PF_HIT = 7;
	constexpr auto PF_CLIMB = 13;

	constexpr auto PF_DEATH = 7;	// ���[�V�������オ���Ă��Ă��Ȃ����ߑ�p

	// �`��؂�ւ��܂łɕK�v�ȃt���[����
	constexpr auto MF_INTERVAL = 4;

	// �Đ����ԁiPlayer Motion Frame�j
	constexpr auto PMF_IDOL = PF_IDOL * MF_INTERVAL;
	constexpr auto PMF_RUN = PF_RUN * 3;
	constexpr auto PMF_DASH = PF_DASH * MF_INTERVAL;
	constexpr auto PMF_JUMP = PF_JUMP * MF_INTERVAL;
	constexpr auto PMF_FALL = PF_FALL * MF_INTERVAL;
	constexpr auto PMF_ROB = PF_ROB * MF_INTERVAL;
	constexpr auto PMF_GIVE = PF_GIVE * MF_INTERVAL;
	// constexpr auto PMF_HIT = PF_HIT * MF_INTERVAL;
	constexpr auto PMF_HIT = 60;
	constexpr auto PMF_DEATH = 60;
	constexpr auto PMF_CLIMB = PF_CLIMB * MF_INTERVAL;
}

namespace inr {

	Player::Player(Game& game) : ObjectBase::ObjectBase(game) {
		_type = ObjectType::PLAYER;
		_aState = ActionState::IDOL;

		_aCount = 0;
		_aFrame = 0;
		_sounds = 0;
		_knockBack = 0;
		_dashInterval = 0;
		_judegFrame = 0;

		_souls;
		// _souls.push(nullptr);

		_direction = false;
		_changeGraph = true;
		_input = true;
		_jumpPower = 0;
		_position = {0, 0};
		_divKey = std::make_pair(PKEY_IDOL, key::SOUND_NUM);
		_moveVector = { 0, 0 };
		// _moveVector = std::make_pair(0, 0);
		
		// _mainCollision = { _position, (PLAYER_WIDTH / 2) - BF_WIDTH, PLAYER_WIDTH / 2, (PLAYER_HIGHT / 2) - BF_HEIGHT_MIN, (PLAYER_HIGHT / 2) + BF_HEIGHT_MAX , true};
		_mainCollision = { _position, (PLAYER_WIDTH / 2), (PLAYER_HEIGHT / 2), true };
		
		
		//(_position, PLAYER_WIDTH / 2, PLAYER_HIGHT / 2);
		Init();
	}

	Player::~Player() {
		_motionKey.clear();
		_collisions.clear();
	}

	void Player::Init() {
		_invincible = 0;	// ���G����
		_pal = 255;

		// �L�[���@first:�A�j���[�V�����̑��t���[�����Asecond:SE�̍Đ��t���[����
		_motionKey = {
					{PKEY_DEATH, {PMF_DEATH, 50}},	// ���S����
					{PKEY_IDOL, {PMF_IDOL, SE_NUM}}, 
					{PKEY_RUN, {PMF_RUN, SE_RUN1}}, 
					{PKEY_DASH, {PMF_DASH, 50}},
					{PKEY_JUMP, {PMF_JUMP, 50}},
					{PKEY_FALL, {PMF_FALL, 50}},
					{PKEY_ROB, {PMF_ROB, 10}}, 
					{PKEY_GIVE, {PMF_GIVE, 10}},
					{PKEY_HIT, {PMF_HIT, 50}},
					{PKEY_CLIMB, {PMF_CLIMB, 0}},
		};

		auto x = _position.GetX();
		auto y = _position.GetY();


		// AABB dashBox = { _position, DASH_WIDTH1, DASH_WIDTH2, DASH_HEIGHT1, DASH_HEIGHT2 };
		AABB dashBox = { _position, 35, 40, true };
		AABB robBox = { _position, ROB_WIDTH1, ROB_WIDTH2, ROB_HEIGHT1, ROB_HEIGHT2 };
		AABB giveBox = { _position, GIVE_WIDTH1, GIVE_WIDTH2, GIVE_HEIGHT1, GIVE_HEIGHT2 };
		
		_collisions = { {PKEY_ROB, {robBox}},
						{PKEY_GIVE, {giveBox}},
						{PKEY_DASH, {dashBox}} };
}

	void Player::Process() {
		if(_aState != ActionState::DEATH) ObjectBase::Process();

		if (Dead() == true) return;
		// ���͏����擾
		auto leverLR = _game.GetLeverLR();
		auto leverUD = _game.GetLeverUD();
		auto key = _game.GetTrgKey();

		/*Move();
		Climb();*/
		Move(leverLR); // �ړ������i���o�[���͎�t�̂��ߒ�~���j
		Climb(leverUD);	// �㉺�ړ��i���o�[���͎�t�̂��ߒ�~���j
		Action(key); // �A�N�V����
		Gran();
		Dash();	// �_�b�V��
		Jump(); // �W�����v����

		// �ʒu���W�̍X�V
		PositionUpdate();

		// �e��Փˏ���
		auto&& objs = _game.GetObjectServer()->GetEnemys();

		auto cBox = _collisions.find(_divKey.first);
		if (cBox != _collisions.end()) {
			for (auto&& obj : objs) {
				obj->CollisionHit(_divKey.first, cBox->second, _direction);
			}
		}
		auto&& crystal = _game.GetObjectServer()->GetGimmicks();
		if (cBox != _collisions.end()) {
			for (auto obj : crystal) {
				if (obj->GimmickType() != gimmick::CRYSTAL) continue;
				std::dynamic_pointer_cast<Crystal>(obj)->CollisionHit(_divKey.first, cBox->second, _direction);
			}
		}
	}

	void Player::Draw() {
		 Vector2 xy = _position;
		 _game.GetMapChips()->Clamp(xy);
		 auto x = xy.IntX();
		 auto y = xy.IntY();

		/*DrawFormatString(1500, 100, GetColor(0, 255, 0), "�`����Wx = %d", x);
		DrawFormatString(1500, 150, GetColor(0, 255, 0), "�`����Wy = %d", y);*/

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		/*std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}*/

#ifdef _DEBUG
		// DebugInfo();
#endif
	}

	void Player::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];

		auto it = _collisions.find(_divKey.first);
		if (it != _collisions.end()) it->second.Update(_position, _direction);
		if (_souls.empty() == true) {
			auto addSoul = _souls;
			for (auto i = 0; i < _souls.size(); ++i) {
				_game.GetObjectServer()->Add(addSoul.front());
				addSoul.pop();
			}
		}

		_mainCollision.Update(_position, _direction);

	}

	void Player::SetParameter(Vector2 newpos) {
		_oValue.PositionsUpdate(newpos);
		_position = newpos;
		// ������ł͂Ȃ��ꍇ�A�Ή����鍰���I�u�W�F�N�g�T�[�o�[�ɍēo�^����
		if (_souls.empty() != true) {
			auto addSoul = _souls;
			for (auto i = 0; i < _souls.size(); ++i) {
				addSoul.front()->SetSpwan(_position);
				_game.GetObjectServer()->Add(std::move(addSoul.front()));
				addSoul.pop();
			}
		}

		auto it = _collisions.find(_divKey.first);
		if (it != _collisions.end()) it->second.Update(_position, _direction);

		_mainCollision.Update(_position, _direction);
	}

	void Player::StateUpdate() {
		if (0 < _invincible) --_invincible;	// ���G���Ԃ�����ꍇ�͌���������
		// �e�탂�[�V�����ɍ��킹���C��
		switch (_aState) {
		// �ړ���
		case ActionState::MOVE:
			if (!_stand) {
				ChangeState(ActionState::FALL, PKEY_FALL);
			}
			
			return;
		// �W�����v��
		case ActionState::JUMP:
			// �����l���v���X�̏ꍇ
			if (0 < _gravity) {
				_changeGraph = true;
				_aState = ActionState::FALL;
				_divKey.first = PKEY_FALL;
			}
			else if (_stand) {	// �d�͉������Ȃ��ꍇ�̓A�C�h����ԂɑJ��
				ChangeState(ActionState::IDOL, PKEY_IDOL);
			}
			if (_gran) _aState = ActionState::GRAN;
			return;
		// ������
		case ActionState::FALL:
			// �����Ă��Ă�ꍇ
			if (_stand) {
			//if (_stand && _gravity == 0) {
				//// ���n����炷
				auto land = SoundResearch(key::SOUND_PLAYER_FALL);
				auto soundType = se::SoundServer::GetPlayType(_divKey.second);
				PlaySoundMem(land, soundType);
				ChangeState(ActionState::IDOL, PKEY_IDOL);
			}
			if (_gran) _aState = ActionState::GRAN;	// �͂ݏ�ԂɑJ�ڂ���
			return;
		// �D���A�N�V������
		case ActionState::ROB:
			// �`��X�V������ꍇ�͏������甲����
			if (_changeGraph == true) break;
			if(0 <= _judegFrame) --_judegFrame;	// �J�E���^����
			// ����J�E���^�̓[���ɂȂ������H
			if (_judegFrame == 0) {
				auto it = _collisions.find(_divKey.first);
				// �����蔻����~�߂�
				it->second.GetCollisionFlgB() = false;
#ifdef _DEBUG
				it->second.GetbDrawFlg() = false;
#endif
			} 
			// �A�j���[�V�����J�E���^�̓}�b�N�X�ɂȂ������H
			if (AnimationCountMax()) {
				ChangeState(ActionState::IDOL, PKEY_IDOL);
				_input = true;	// ���͂��󂯕t����
			}
			break;
		case ActionState::GIVE:
			if (_changeGraph == true) break;
			if (0 <= _judegFrame) --_judegFrame;	// �J�E���^����
			// ����J�E���^�̓[���ɂȂ������H
			if (_judegFrame == 0) {
				auto it = _collisions.find(_divKey.first);
				// �����蔻����~�߂�
				it->second.GetCollisionFlgB() = false;
#ifdef _DEBUG
				it->second.GetbDrawFlg() = false;
#endif
			}
			// �A�j���[�V�����J�E���^�̓}�b�N�X�ɂȂ������H
			if (AnimationCountMax()) {
				ChangeState(ActionState::IDOL, PKEY_IDOL);
				_input = true;	// ���͂��󂯕t����
			}
			break;
		// �m�b�N�o�b�N��
		case ActionState::HIT:
			if (_invincible == HIT_FRAME) {
				ChangeState(ActionState::IDOL, PKEY_IDOL);
				_input = true;
				break;
			}
			_moveVector.GetPX() = _knockBack / HIT_FRAME;	// �ړ���
			break;

		case ActionState::GRAN:
			if (_gran == false || _input == false) {
				_gran = false;
				if (0 <= _gravity) ChangeState(ActionState::FALL, PKEY_FALL);
				else if(_stand == true) ChangeState(ActionState::IDOL, PKEY_IDOL);
			}
			break;
		default:
			return;
		}
	}

	bool Player::Action(int key) {
		// �O�t���[���̏��
		auto beforeState = _aState;
		auto x = _position.GetX();
		auto y = _position.GetY();

		// key�ɓ��͏�񂪂���ꍇ
		if (key) {
			// ���͏��ɉ��������������s
			switch (key) {
			case PAD_INPUT_1:	// X�{�^���������ꂽ�ꍇ�A�u����D���v
				// �O�t���[���̏�ԂƓ����ꍇ�͏������甲����
				// if (_aState == ActionState::ROB || _aState == ActionState::GIVE) break;
				Rob(x, y); //�@�D���A�N�V�������s
				break;
			case PAD_INPUT_2:	// Y�{�^���������ꂽ�ꍇ�A�u����^����v
				if (_aState == ActionState::GIVE || _aState == ActionState::ROB) break;
				Give(x, y);		// �^����A�N�V�������s
				break;
			//case PAD_INPUT_3:	// A�{�^���������ꂽ�ꍇ�A�u�W�����v�v
			//	InputJump();
			//	// 
			//	//if (_aState != ActionState::IDOL && _aState != ActionState::MOVE || _aState == ActionState::JUMP) break;
			//	Jump();

			case PAD_INPUT_5:	// L1�������ꂽ�ꍇ�A�u����؂�ւ���v
				ChangeSoul();
				break;
			case PAD_INPUT_6:	// R1�������ꂽ�ꍇ�A�u�_�b�V���v
				if (_aState == ActionState::HIT) break;
				InputDash(x);
				break;
			}
		}

		// �ً}����

		//if (CheckHitKey(KEY_INPUT_A) == TRUE) Rob(x, y); //�@�D���A�N�V�������s
		//else if (CheckHitKey(KEY_INPUT_S) == TRUE) {
		//	if (_aState == ActionState::GIVE || _aState != ActionState::ROB) Give(x, y);		// �^����A�N�V�������s
		//}
		//else if (CheckHitKey(KEY_INPUT_D) == TRUE)ChangeSoul();
		//else if (CheckHitKey(KEY_INPUT_F) == TRUE) if (_aState != ActionState::HIT) InputDash(x);

		// �A�C�h����ԈȊO�ŁA�A�j���[�V�������I����ĂȂ��ꍇ
		StateUpdate();
		
				/*if (!_speed && _aCount == 0) {
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
				}*/

		return false;
	}

	void Player::IsGran() {
		// �ӂƐڐG���Ă��邩�ǂ���
		if (_input != true) return;	//�@���͂��󂯕t���Ă��Ȃ��ꍇ�́A�͂߂Ȃ�
		if (_gran == true) return;
		// B�{�^�����͂��������ꍇ�A�ӓo���ԂɑJ�ڂ���
		if (_game.GetTrgKey() == PAD_INPUT_4 || CheckHitKey(KEY_INPUT_UP) == TRUE) {
			ChangeState(ActionState::GRAN, PKEY_CLIMB);
			_gran = true;
		}
	}

	void Player::Gran() {
		if (_game.GetMapChips()->HitIvy(NowCollision(_divKey.first), _position, _moveVector, _direction)) IsGran();
		else _gran = false;
	}

	void Player::Move(int lever) {
		// ���͉\���H
		if (_input == true) {
			// ��Ԃ��A�C�h���A�܂��̓��[�u�̎������ړ��������s���B
			auto direction = _direction;
			if (lever < -10) _direction = PL_LEFT;
			else if (10 < lever) _direction = PL_RIGHT;

			// �������ς�����ꍇ�̓t���O��؂�ւ���
			if (_direction != direction) _changeDirection = true;
			if (_aState != ActionState::GRAN) {

				if (_aState != ActionState::FALL && _aState == ActionState::IDOL || _aState == ActionState::MOVE) {
					// ���͏�񂪂���ꍇ
					if (lever < -100 || 100 < lever) {
						// move�ł͂Ȃ����A�L�[�Ə�Ԃ��X�V
						if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
							ChangeState(ActionState::MOVE, PKEY_RUN);
						}
						// SE�̊Ǘ�
						if (_aCount % GetSoundFrame(_divKey.first) == 0) {
							auto sound1 = SoundResearch(key::SOUND_PLAYER_RUN1);
							auto soundType = se::SoundServer::GetPlayType(_divKey.second);
							PlaySoundMem(sound1, soundType);
						}
						// return;
						// �����Ă��Ă����͂��Ȃ��ꍇ
					}
					else if (_aState == ActionState::MOVE) {
						switch (_stand) {
						case true:	// �����Ă���ꍇ
							ChangeState(ActionState::IDOL, PKEY_IDOL);
							_speed = 0;
							break;
							//case false:	// ������Ԃ̏ꍇ
							//	ChangeState(ActionState::FALL, PKEY_FALL);
							//	_speed = 0;
							//	break;
						}
						return;
					}
				}
				// ���W�ύX
				_speed = (lever * MAX_SPPED) / 1000;
				// �ړ��x�N�g�����
				_moveVector.GetPX() = 1.0 * _speed;
				_speed = 0;
			}
		}
	}

	void Player::Move() {
		// ���͉\���H
		if (_input == true) {
			// ��Ԃ��A�C�h���A�܂��̓��[�u�̎������ړ��������s���B
			auto direction = _direction;
			if (CheckHitKey(KEY_INPUT_LEFT) == TRUE) _direction = PL_LEFT;
			else if (CheckHitKey(KEY_INPUT_RIGHT) == TRUE) _direction = PL_RIGHT;

			// �������ς�����ꍇ�̓t���O��؂�ւ���
			if (_direction != direction) _changeDirection = true;
			if (_aState != ActionState::GRAN) {

				if (_aState != ActionState::FALL && _aState == ActionState::IDOL || _aState == ActionState::MOVE) {
					// ���͏�񂪂���ꍇ
					if (CheckHitKey(KEY_INPUT_LEFT) == TRUE || CheckHitKey(KEY_INPUT_RIGHT) == TRUE) {
						// move�ł͂Ȃ����A�L�[�Ə�Ԃ��X�V
						if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
							ChangeState(ActionState::MOVE, PKEY_RUN);
						}
						// SE�̊Ǘ�
						if (_aCount % GetSoundFrame(_divKey.first) == 0) {
							auto sound1 = SoundResearch(key::SOUND_PLAYER_RUN1);
							auto soundType = se::SoundServer::GetPlayType(_divKey.second);
							PlaySoundMem(sound1, soundType);
						}
						// return;
						// �����Ă��Ă����͂��Ȃ��ꍇ
						if (CheckHitKey(KEY_INPUT_LEFT) == TRUE) _moveVector.GetPX() = -8.0;
						else if (CheckHitKey(KEY_INPUT_RIGHT) == TRUE) _moveVector.GetPX() = 8.0;
					}
					else if (_aState == ActionState::MOVE) {
						switch (_stand) {
						case true:	// �����Ă���ꍇ
							ChangeState(ActionState::IDOL, PKEY_IDOL);
							_speed = 0;
							break;
							//case false:	// ������Ԃ̏ꍇ
							//	ChangeState(ActionState::FALL, PKEY_FALL);
							//	_speed = 0;
							//	break;
						}
						return;
					}
				}
				// ���W�ύX
				// _speed = (lever * MAX_SPPED) / 1000;
				// �ړ��x�N�g�����
				_speed = 0;
			}
		}
	}

	void Player::InputDash(double x) {
		// ���͉\��Ԃ��A�C���^�[�o�����ł͂Ȃ����H
		if (_input == true && !_dashInterval) {
			// �_�b�V����Ԃł͂Ȃ��ꍇ�A�e�평�������������s
			if (_aState != ActionState::DASH) {
				ChangeState(ActionState::DASH, PKEY_DASH);
				auto soundKey = SoundResearch(key::SOUND_PLAYER_ROB);
				// auto soundKey = SoundResearch(key::SOUND_PLAYER_DASH);
				// auto soundType = se::SoundServer::GetPlayType(_divKey.second);
				PlaySoundMem(soundKey, DX_PLAYTYPE_BACK);
				// auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
				// PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
				// �_�b�V���A�N�V������̍��W������o���i�G or �}�b�v�`�b�v�ɐڐG�����ꍇ�͂��̌���ł͂Ȃ��j
				// (_direction == PL_LEFT) ? _dashX = x - DASH_MAX : _dashX = x + DASH_MAX;
				/*auto it = _collisions.find(_divKey.first);
				it->second.GetCollisionFlgB() = true;*/
				_dashX = DASH_MAX;
				_input = false;	// ���A�N�V�����̓��͂��~����
				_gran = false;

				auto eff = std::make_unique<EffectBase>(_game.GetGame(), effect::DASH, _position, 30, _direction);
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eff));
			}
		}
	}

	void Player::InputJump() {
		// ���͉\��Ԃ��A�����Ă���ꍇ�̂ݎ��s�\
		if (_input == true && _stand) {
			_jumpPower += 1;	// ���߃J�E���^�𑝂₷
		}
	}

	// �_�b�V������
	void Player::Dash() {
		// �C���^�[�o��������ꍇ�͌��炷
		if (0 < _dashInterval) --_dashInterval;
		// �_�b�V����Ԃł͂Ȃ��ꍇ�A�����𒆒f
		if (_input == true) return;

		if (_aState == ActionState::DASH) {
			double dashVec;	// �ړ��x�N�g��
		// �����Ă�������ɉ����đ������x�N�g����؂�ւ�
			(_direction == PL_LEFT) ? dashVec = -DASH_VEC : dashVec = DASH_VEC;
			_moveVector.GetPX() = dashVec;
			_dashX -= DASH_VEC;

			auto nextPos = _position.GetX() + dashVec;	// �ړ���̍��W���擾
			_gravity = 0;	// �_�b�V�����͏d�͏�������
			bool moved = _dashX <= 0;	

			// �_�b�V�������͊����������ǂ����H
			if (moved) {
				_input = true;
				_dashInterval = DASH_INTERVAL;
				// �����Ă��邩�ǂ����Ŏ��̃��[�V�����𔻒�
				if (_stand) {
					ChangeState(ActionState::IDOL, PKEY_IDOL);
					return;
				}
				else {
					ChangeState(ActionState::FALL, PKEY_FALL);
				}
			}
		}
	}

	void Player::Climb(int leverUD) {
		if (_aState != ActionState::GRAN) return;	// �͂݃t���O���I���ł͂Ȃ��ꍇ�͏������s��Ȃ�
		if (-50 < leverUD && leverUD < 50) return;		
		// ���W�ύX
		double spd = (leverUD * MAX_SPPED) / 1000.0;
		// �ړ��x�N�g�����
		_moveVector.GetPY() = 1.0 * spd;
	}

	void Player::Climb() {
		if (_aState != ActionState::GRAN) return;	// �͂݃t���O���I���ł͂Ȃ��ꍇ�͏������s��Ȃ�
		if (CheckHitKey(KEY_INPUT_UP) == TRUE) _moveVector.GetPY() = 3.0;
		else if (CheckHitKey(KEY_INPUT_DOWN) == TRUE) _moveVector.GetPY() = -3.0;
	}

	void Player::Jump() {
		if ((_input == true && _stand) || _gran == true) {
				// ���߂͂��邩�H
					auto pressKey = _game.GetKey();
					if (pressKey & PAD_INPUT_3 || CheckHitKey(KEY_INPUT_SPACE) == TRUE) {
						// ���߃J�E���^�𑝂₷
						_jumpPower += 1;
						// ���߃J�E���^���}�b�N�X�ł͂Ȃ��ꍇ�A�������甲����
						if (_jumpPower < JUMP_MAX) {
							return;
						}
					}  if (_jumpPower) {
						// A�L�[�̓��͂��Ȃ��ꍇ�A�W�����v�����s
						ChangeState(ActionState::JUMP, PKEY_JUMP);
						auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
						PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

						auto eff = std::make_unique<EffectBase>(_game.GetGame(), effect::JUMP, _position, 30);
						_game.GetModeServer()->GetModeMain()->GetEffectServer()->Add(std::move(eff));

						// �򋗗����Z�o
						auto jumpPower = JUMP_VECTOR * (1.0 + _jumpPower);
						// �򋗗����ő�l�𒴂����ꍇ�͏C��
						if (JUMP_MAX < jumpPower) jumpPower = JUMP_MAX;
						// �W�����v�̔򋗗���o�^
						// ���̒l�͒n�ʂɐG�ꂽ or �V��ɐڐG�����ꍇ�A0�ɂ���B
						_gravity = -jumpPower;
						_gran = false;
					}
			}
		//if(_jumpPower) _jumpPower = 0;
		_jumpPower = 0;
	}

	void Player::Rob(double x, double y) {
		if (_input == true) {
			ChangeState(ActionState::ROB, PKEY_ROB);
			// SE�ǂݍ���
			auto sound1 = SoundResearch(key::SOUND_PLAYER_ROB);
			PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

			auto it = _collisions.find(PKEY_ROB);
			// it->second.SetVector(amin, amax);
			it->second.GetCollisionFlgB() = true;	// ����I��
#ifdef _DEBUG
			it->second.GetbDrawFlg() = true;
#endif

			}
			_input = false; // ���͂��󂯕t���Ȃ�����
			_judegFrame = ROB_JUDGEMENT;	// ����J�E���^
		
	}

	void Player::Give(double x, double y) {

		if (_input == true) {
			ChangeState(ActionState::GIVE, PKEY_GIVE);
			// SE�ǂݍ���
			auto sound1 = SoundResearch(key::SOUND_PLAYER_GIVE);
			PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

			auto it = _collisions.find(PKEY_GIVE);
			it->second.GetCollisionFlgB() = true;	// ����I��
#ifdef _DEBUG
			it->second.GetbDrawFlg() = true;
#endif
			_input = false; // ���͂��󂯕t���Ȃ�����
			_judegFrame = ROB_JUDGEMENT;	// ����J�E���^
		}
	}

	void Player::ChangeSoul() {
		if (_input == false) return;
		if (2 <= _souls.size()) {
			_souls.push(_souls.front());
			_souls.pop();
		}
	}

	bool Player::Damage(bool mv) {
		// ���G���Ԓ��ł͂Ȃ��ꍇ�̂݁A
		if (_invincible == 0) {
			// �_���[�W����
			_input = false;	// ���͏�����e��

			if (_souls.empty() == false) {
				_souls.front()->Del();
				_souls.pop();

				ChangeState(ActionState::HIT, PKEY_HIT);	// ��ԑJ��
				auto soundKey = SoundResearch(key::SOUND_PLAYER_HIT);
				auto soundType = se::SoundServer::GetPlayType(_divKey.second);
				PlaySoundMem(soundKey, se::SoundServer::GetPlayType(_divKey.second));
				
			}
			else {
				// ��̏ꍇ�͎��S�������s��
				Death();
				return true;
			}

			// �m�b�N�o�b�N�ʁi�����̐ݒ�j
			switch (mv) {
			// ���ɋ���ꍇ
			case false:
				_knockBack = HIT_MAX;
				break;
			// �E�ɋ���ꍇ
			case true:
				_knockBack = -HIT_MAX;
				break;
			}
			_invincible = INVINCIBLE_TIME;	// ���G���Ԃ�ݒ�
			return true;
		}
		return false;
	}

	bool Player::Debuf() {
		// ���@�̓A�C�e����ێ����Ă��邩�H
		// �ێ����Ă��Ȃ��ꍇ�A��莞�Ԃ̊Ԉړ����x�Ƀ}�C�i�X�␳��������
		return true;
	}

	bool Player::Dead() {
		if (_aState == ActionState::DEATH) {
			if (IsAnimationMax() == true) {
				_game.GetModeServer()->GetModeMain()->GameOver();	// ���݂̃X�e�[�W�̏��������s��
			} else AnimationCount();
			return true;
		}
		else AnimationCount();
		return false;
	}

	void Player::Death() {
		auto sound = se::SoundServer::GetSound(key::SOUND_PLAYER_DEAD);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		// ���S�����i�A�j���[�V���������ށj
		ChangeState(ActionState::DEATH, PKEY_DEATH);	// ���S��Ԃɐ؂�ւ���
		_input = false;	// ���͂𖳌�������
		_mainCollision.GetCollisionFlgB() = false;	// �����蔻�肪�@�\���Ȃ��悤�ɃI�t�ɂ���
		auto box = _collisions.find(PKEY_DASH);
		box->second.GetCollisionFlgB() = false;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
		box->second.GetbDrawFlg() = false;
#endif
	}

	void Player::AnimationInit() { 
		if (_aState != ActionState::IDOL && _aState !=ActionState::MOVE && _aState != ActionState::JUMP && _aState!= ActionState::FALL && _aState!=ActionState::DASH) {
			// �ȉ��̃R�[�h�͏C���\��
			// �t���O���I�t�ɂ���
			auto it = _collisions.find(_divKey.first);
			it->second.GetbDrawFlg() = false;
		}
		_aCount = 0;
	}

	AABB Player::GetAABB() {
		auto dx = _position.GetX();
		auto dy = _position.GetY();
		Vector2 minV(dx - PLAYER_WIDTH / 2, dy + PLAYER_HEIGHT / 2);
		Vector2 maxV(dx + PLAYER_WIDTH / 2, dy - PLAYER_HEIGHT / 2);
		AABB playerAABB(minV, maxV);

		return playerAABB;
	}

	// �ʒu���W�X�V
	void Player::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		if (_aState != ActionState::GRAN) _moveVector.GetPY() = _gravity;
		auto isGran = false;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		auto hitchip = _game.GetMapChips()->IsHit(NowCollision(_divKey.first), _position, _moveVector, _direction, &isGran);
		if (hitchip == mapchip::THORM) DamageThorm();	// ������ڐG�����ꍇ���_���[�W�������Ăяo��
		// �ӂɐڐG���Ă���ꍇ�̂ݏ��������s����
		// �M�~�b�N�ɂ߂荞��ł��邩�H
		GimmickCheck(_moveVector);
		

		_position = _position + _moveVector;	// �ʒu���W���X�V

		auto mcsw = _game.GetMapChips()->GetMapSizeWidth();

		if (_mainCollision.GetMin().GetX() < 0) {
			_position.GetPX() = _mainCollision.GetWidthMax();
		}
		else if (_game.GetMapChips()->GetMapSizeWidth() < _mainCollision.GetMax().GetX() && 0 < _moveVector.GetX()) {
			_position.GetPX() = mcsw - _mainCollision.GetWidthMax();
		}

		// �e�퓖���蔻��̍X�V
		auto it = _collisions.find(_divKey.first);
		if (it != _collisions.end()) {
			it->second.Update(_position, _direction);
		}

		if (_aState == ActionState::DASH) {
			_mainCollision.Swap(it->second);
		} else {
			// ���������ŏC���������Ă���̂������ŉ�ꂽ
			_mainCollision.Update(_position, _direction);
		}

		// ���̖ڕW������ύX
		if (!_souls.empty()) {
			auto ss = _souls;
			for (auto i = 1; i <= _souls.size(); ++i) {
				ss.front()->Space(i * 60);
				ss.pop();
			}
		}

		_game.GetMapChips()->WorldUpdate(_position);
		// �ړ��x�N�g��������
		_moveVector = { 0, 0 };
	}

	void Player::ChangeState(Player::ActionState nextState, std::string key) {
		_aState = nextState;
		_divKey.first = key;
		_changeGraph = true;
	}

	void Player::SoulCatch(std::shared_ptr<SoulSkin> soul) {
		_souls.push(std::move(soul));
	}

	std::shared_ptr<SoulSkin> Player::GiveSoul() {
		auto givesoul = _souls.front();	// �Q�Ƃ����o��
		_souls.pop();
		return std::move(givesoul);
	}

	bool Player::IsStandChip() {
		// ��l����
		auto nowcol = NowCollision(_divKey.first);
		auto chipType = _game.GetMapChips()->IsStand(nowcol, _position, _gravity, &_lastChip);
		switch (chipType) {
		case mapchip::IVY:
		case mapchip::NONE:
			return false;
		case mapchip::THORM:
			// ��΂��������Ăяo��
			DamageThorm();
		default:
			return true;
		}
	}

	void Player::DamageThorm() {
		// ���������
		if (_souls.empty() == false) {
			_souls.front()->Del();
			_souls.pop();

			auto soundKey = SoundResearch(key::SOUND_PLAYER_HIT);
			auto soundType = se::SoundServer::GetPlayType(_divKey.second);
			PlaySoundMem(soundKey, se::SoundServer::GetPlayType(_divKey.second));
		}
		else {
			Death();	//�@���S����
			return;
		}
		_invincible = INVINCIBLE_TIME;	// ���G���Ԃ�ݒ�
		_position = _lastChip;	// ���W�؂�ւ�
		// �ړ���Ƀo�O���������Ȃ��悤�ɒ���
		_knockBack = 0;
		_dashX = 0;
		_gravity = 0;

	}

	bool Player::Reset() {
		// �e�평�����������s
		_position = _oValue.Positions().at(0);
		_mainCollision.Update(_position, _direction);
		_mainCollision.GetCollisionFlgB() = true;
		auto dashcol = _collisions.find(PKEY_DASH);
		dashcol->second.Update(_position, _direction);
		dashcol->second.GetCollisionFlgB() = true;

#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = true;
		dashcol->second.GetbDrawFlg() = true;
#endif

		ChangeState(ActionState::IDOL, PKEY_IDOL);
		_input = true;
		_gran = false;
		return true;
	}

	AABB Player::NowCollision(std::string key) {
		// ���݂̃A�N�V������Ԃ̓{�b�N�X���C������K�v�����邩�H
		if (_aState == ActionState::DASH) {
			auto it = _collisions.find(key);
			return it->second;
		}
		return _mainCollision;
	}



	// �f�o�b�O�p�����i�ϐ��̕\���E�����蔻��̕`�擙�j
#ifdef _DEBUG
	void Player::DebugInfo() {
		DrawFormatString(0, 0, GetColor(255, 0, 255), "ActionStatet : %d\n", _aState);
		DrawFormatString(0, 25, GetColor(255, 0, 255), "Animation : %d\n", _aCount);
		ObjectBase::DrawDebugBox(_mainCollision);
		DrawFormatString(0, 100, GetColor(255, 0, 0), "maincollision�iminX:%d, minY:%d�j\n", _mainCollision.GetMin().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 125, GetColor(255, 0, 0), "maincollision�imaxX:%d, maxY:%d�j\n", _mainCollision.GetMax().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 150, GetColor(255, 0, 0), "pos.x = %d\n", _position.IntX());
		DrawFormatString(0, 175, GetColor(255, 0, 0), "pos.y = %d\n", _position.IntY());
		DrawFormatString(0, 200, GetColor(255, 0, 0), "stand = %d\n", _stand);
		DrawFormatString(0, 225, GetColor(255, 0, 0), "_gravity = %d\n", _gravity);
	}
#endif
}