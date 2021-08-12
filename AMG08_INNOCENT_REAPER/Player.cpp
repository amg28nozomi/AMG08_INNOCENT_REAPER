#include "Player.h"
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "MapChips.h"
#include <DxLib.h>
#include <unordered_map>

namespace {
#ifdef _RELEASE
	constexpr auto START_POSITION_X = inr::WINDOW_H / 2;
	constexpr auto START_POSITION_Y = inr::WINDOW_W / 2;
#endif
#ifdef _DEBUG
	constexpr auto START_POSITION_X = 100;
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
	constexpr auto ROB_WIDTH1 = 20 + 50; //-75;
	constexpr auto ROB_WIDTH2 = -20;
	constexpr auto ROB_HEIGHT1 = -20;
	constexpr auto ROB_HEIGHT2 = 60;

	// �^����A�N�V�����̓����蔻��
	constexpr auto GIVE_WIDTH1 = 60;
	constexpr auto GIVE_WIDTH2 = -10;
	constexpr auto GIVE_HEIGHT1 = -30;
	constexpr auto GIVE_HEIGHT2 = 70;

	// �W�����v�A�N�V����
	constexpr auto JUMP_VECTOR = 1;	// �W�����v�̈ړ��x�N�g��
	constexpr auto JUMP_MAX = 15;
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


	// �e�탂�[�V�����̉摜��
	constexpr auto PF_IDOL = 13;
	constexpr auto PF_RUN = 9;
	constexpr auto PF_DASH = 13;
	constexpr auto PF_JUMP = 6;
	constexpr auto PF_FALL = 10;
	constexpr auto PF_ROB = 13;
	constexpr auto PF_GIVE = 16;
	constexpr auto PF_HIT = 7;

	// �`��؂�ւ��܂łɕK�v�ȃt���[����
	constexpr auto MF_INTERVAL = 4;

	// �Đ����ԁiPlayer Motion Frame�j
	constexpr auto PMF_IDOL = PF_IDOL * MF_INTERVAL;
	constexpr auto PMF_RUN = PF_RUN * MF_INTERVAL;
	constexpr auto PMF_DASH = PF_DASH * MF_INTERVAL;
	constexpr auto PMF_JUMP = PF_JUMP * MF_INTERVAL;
	constexpr auto PMF_FALL = PF_FALL * MF_INTERVAL;
	constexpr auto PMF_ROB = PF_ROB * MF_INTERVAL;
	constexpr auto PMF_GIVE = PF_GIVE * MF_INTERVAL;
	constexpr auto PMF_HIT = PF_HIT * MF_INTERVAL;
}

namespace inr {

	Player::Player(Game& game) : ObjectBase::ObjectBase(game) {
		_type = ObjectType::PLAYER;
		_aState = ActionState::IDOL;

		_aCount = 0;
		_aFrame = 0;
		_sounds = 0;
		_direction = false;
		_changeGraph = true;
		_jumpPower = 0;
		_position = { START_POSITION_X, START_POSITION_Y};
		_divKey = std::make_pair(PKEY_IDOL, key::SOUND_NUM);
		_moveVector = { 0, 0 };
		// _moveVector = std::make_pair(0, 0);
		_mainCollision = { _position, (PLAYER_WIDTH / 2) - BF_WIDTH, PLAYER_WIDTH / 2, (PLAYER_HIGHT / 2) - BF_HEIGHT_MIN, (PLAYER_HIGHT / 2) + BF_HEIGHT_MAX };
		//(_position, PLAYER_WIDTH / 2, PLAYER_HIGHT / 2);
		Init();
	}

	Player::~Player() {
		_motionKey.clear();
	}

	void Player::Init() {
		// �L�[���@first:�A�j���[�V�����̑��t���[�����Asecond:SE�̍Đ��t���[����
		_motionKey = {{PKEY_IDOL, {PMF_IDOL, SE_NUM}}, 
					{PKEY_RUN, {PMF_RUN, SE_RUN1}}, 
					{PKEY_DASH, {PMF_DASH, 50}},
					{PKEY_JUMP, {PMF_JUMP, 50}},
					{PKEY_FALL, {PMF_FALL, 50}},
					{PKEY_ROB, {PMF_ROB, 10}}, 
					{PKEY_GIVE, {PMF_GIVE, 10}},
					{PKEY_HIT, {PMF_HIT, 50}},
		};

		auto x = _position.GetX();
		auto y = _position.GetY();


		AABB dashBox = { _position, DASH_WIDTH1, DASH_WIDTH2, DASH_HEIGHT1, DASH_HEIGHT2 };
		AABB robBox = { _position, ROB_WIDTH1, ROB_WIDTH2, ROB_HEIGHT1, ROB_HEIGHT2 };
		AABB giveBox = { _position, GIVE_WIDTH1, GIVE_WIDTH2, GIVE_HEIGHT1, GIVE_HEIGHT2 };
		
		_collisions = { {PKEY_ROB, {robBox}},
						{PKEY_GIVE, {giveBox}},
						{PKEY_DASH, {dashBox}} };
}

	void Player::Process() {
		ObjectBase::Process();
		// ���͏����擾
		auto leverLR = _game.GetLeverLR();
		auto key = _game.GetTrgKey();

		Move(leverLR); // �ړ�����
		Action(key); // �A�N�V����
		Dash();	// �_�b�V��
		Jump(); // �W�����v����

		// �ʒu���W�̍X�V
		PositionUpdate();
		_game.GetMapChips()->WorldUpdate(_position);
	}

	void Player::Draw() {
		 Vector2 xy = _position;
		 _game.GetMapChips()->Clamp(xy);
		 auto x = xy.IntX();
		 auto y = xy.IntY();

		DrawFormatString(1500, 100, GetColor(0, 255, 0), "�`����Wx = %d", x);
		DrawFormatString(1500, 150, GetColor(0, 255, 0), "�`����Wy = %d", y);

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}

#ifdef _DEBUG
		/*auto dx = _position.GetX();
		auto dy = _position.GetY();
		Vector2 minV(dx - PLAYER_WIDTH / 2, dy + PLAYER_HIGHT / 2);
		Vector2 maxV(dx + PLAYER_WIDTH / 2, dy - PLAYER_HIGHT / 2);
		AABB debugAABB(minV, maxV);

		debugAABB.DrawBox();*/
		DebugInfo();
		ObjectBase::DrawDebugBox(_mainCollision);
		DrawFormatString(0, 100, GetColor(255, 0, 0), "maincollision�iminX:%d, minY:%d�j\n", _mainCollision.GetMin().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 125, GetColor(255, 0, 0), "maincollision�imaxX:%d, maxY:%d�j\n", _mainCollision.GetMax().IntX(), _mainCollision.GetMin().IntY());
		DrawFormatString(0, 150, GetColor(255, 0, 0), "pos.x = %d\n", _position.IntX());
		DrawFormatString(0, 175, GetColor(255, 0, 0), "pos.y = %d\n", _position.IntY());
		DrawFormatString(0, 200, GetColor(255, 0, 0), "stand = %d\n", _stand);
		DrawFormatString(0, 225, GetColor(255, 0, 0), "_gravity = %d\n", _gravity);
#endif

		// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
		if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		else AnimationInit();	// �J�E���g������
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
				if (_aState == ActionState::ROB || _aState == ActionState::GIVE) break;
				Rob(x, y); //�@�D���A�N�V�������s
				break;
			case PAD_INPUT_2:	// Y�{�^���������ꂽ�ꍇ�A�u����^����v
				if (_aState == ActionState::GIVE || _aState == ActionState::ROB) break;
				Give(x, y);		// �^����A�N�V�������s
				break;
			//case PAD_INPUT_3:	// A�{�^���������ꂽ�ꍇ�A�u�W�����v�v
			//	// 
			//	//if (_aState != ActionState::IDOL && _aState != ActionState::MOVE || _aState == ActionState::JUMP) break;
			//	Jump();
			//	break;
			case PAD_INPUT_5:	// L1�������ꂽ�ꍇ�A�u����؂�ւ���v
				break;
			case PAD_INPUT_6:	// R1�������ꂽ�ꍇ�A�u�_�b�V���v
				if (_aState == ActionState::HIT) break;
				InputDash(x);
				break;
			}
		}
		// �A�C�h����ԈȊO�ŁA�A�j���[�V�������I����ĂȂ��ꍇ

		if(_input == false){
			if (_aState != ActionState::IDOL && _stand && _changeGraph != true) {
				if (_aState == ActionState::FALL) {
					auto sound1 = SoundResearch(key::SOUND_PLAYER_FALL);
					auto soundType = se::SoundServer::GetPlayType(_divKey.second);
					PlaySoundMem(sound1, soundType);
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
					_aCount = 0;
				}
				if (!_speed && _aCount == 0) {
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
				}
			}
			// 
			if (_aState == ActionState::JUMP) {
				if (0 <= _gravity) {
					_changeGraph = true;
					_aState = ActionState::FALL;
					_divKey.first = PKEY_FALL;

				}
			}
		}
		return false;
	}

	void Player::Move(int lever) {
		// ���͉\���H
		if (_input != true) {
			// ��Ԃ��A�C�h���A�܂��̓��[�u�̎������ړ��������s���B
			if (_input == false) {
				if (lever < -10) _direction = PL_LEFT;
				else if (10 < lever) _direction = PL_RIGHT;
			}

			if (_aState == ActionState::IDOL || _aState == ActionState::MOVE) {
				// ���͏�񂪂���ꍇ
				if (lever < -100 || 100 < lever) {
					// move�ł͂Ȃ����A�L�[�Ə�Ԃ��X�V
					if (_aState != ActionState::MOVE && _aState != ActionState::JUMP) {
						_changeGraph = true;
						_aState = ActionState::MOVE;
						_divKey.first = PKEY_RUN;
					}
					// _direction = ((0 < lever) ? false : true);
					/* _direction = ((0 < lever) ? true : false);
					_speed = (lever * MAX_SPPED) / 1000;
					Vector2 moveVector = { 1 * _speed, 0 * _speed };
					_position = _position + moveVector; */
					// SE�̊Ǘ�
					if (_aCount % GetSoundFrame(_divKey.first) == 0) {
						auto sound1 = SoundResearch(key::SOUND_PLAYER_RUN1);
						auto soundType = se::SoundServer::GetPlayType(_divKey.second);
						PlaySoundMem(sound1, soundType);
					}
					// return;
					// �����Ă��Ă����͂��Ȃ��ꍇ
				}
				else if (_stand && _aState == ActionState::MOVE) {
					_changeGraph = true;
					_aState = ActionState::IDOL;
					_divKey.first = PKEY_IDOL;
					_speed = 0;
					return;
				}
			}
			// ���W�ύX
			_speed = (lever * MAX_SPPED) / 1000;
			// �ړ������Z�o
			/*double vectory = 0;
			if (JUMP_Y < _moveVector.GetY()) {
				_moveVector.second -= JUMP_Y;
				vectory = -JUMP_Y;
			}*/
			// �P�ʃx�N�g��
			_moveVector.GetPX() = 1.0 * _speed;
			// Vector2 moveVector = { 1 * _speed, vectory };
			// _position = _position + moveVector;
			_speed = 0;
		}
	}

	void Player::InputDash(double x) {
		if (!_dashInterval) {
			// �_�b�V����Ԃł͂Ȃ��ꍇ�A�e�평�������������s
			if (_aState != ActionState::DASH) {
				_aState = ActionState::DASH;
				_divKey.first = PKEY_DASH;
				// auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
				// PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
				// �_�b�V���A�N�V������̍��W������o���i�G or �}�b�v�`�b�v�ɐڐG�����ꍇ�͂��̌���ł͂Ȃ��j
				(_direction == PL_LEFT) ? _lastX = x - DASH_MAX : _lastX = x + DASH_MAX;
				_changeGraph = true;
				_input = true;	// ���A�N�V�����̓��͂��~����
			}
		}
	}

	// �_�b�V������
	void Player::Dash() {
		// �C���^�[�o��������ꍇ�͌��炷
		if (0 < _dashInterval) --_dashInterval;
		// �_�b�V����Ԃł͂Ȃ��ꍇ�A�����𒆒f
		if (_input != true) return;

		double dashVec;	// �ړ��x�N�g��
		// �����Ă�������ɉ����đ������x�N�g����؂�ւ�
		(_direction == PL_LEFT) ? dashVec = -DASH_VEC : dashVec = DASH_VEC;
		_moveVector.GetPX() = dashVec;

		auto nextPos = _position.GetX() + dashVec;	// �ړ���̍��W���擾
		_gravity = 0;	// �_�b�V�����͏d�͏�������
		bool moved;
		(_direction == PL_LEFT) ? moved = nextPos - _lastX <= 0 : moved = _lastX - nextPos <= 0;

		// �_�b�V�������͊����������ǂ����H
		if (moved) {
			_input = false;
			_dashInterval = DASH_INTERVAL;
			_changeGraph = true;
			// �����Ă��邩�ǂ����Ŏ��̃��[�V�����𔻒�
			if (_stand) {
				_aState = ActionState::IDOL;
				_divKey.first = PKEY_IDOL;
				return;
			} else {
				_aState = ActionState::FALL;
				_divKey.first = PKEY_FALL;
			}
		}

	}

	void Player::Jump() {
		// ������Ԃł͂Ȃ��Ƃ�
		if (_stand) {
			// ���͂��󂯕t���Ă��邩�H
			if (_input != true) {
				auto pressKey = _game.GetKey();
				if (pressKey & PAD_INPUT_3) {
					// ���߃J�E���^�𑝂₷
					_jumpPower += 1;
					// ���߃J�E���^���}�b�N�X�ł͂Ȃ��ꍇ�A�������甲����
					if (_jumpPower < JUMP_MAX) {
						return;
					}
				}
				// ���������擾
				// �W�����v�̒l������ꍇ
				if (0 < _jumpPower) {
					// A�L�[�̓��͂��Ȃ��ꍇ�A�W�����v�����s
					_aState = ActionState::JUMP;
					_divKey.first = PKEY_JUMP;
					auto sound = SoundResearch(key::SOUND_PLAYER_JUMP);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
					// �򋗗����Z�o
					auto jumpPower = JUMP_VECTOR * (1.0 + _jumpPower);
					// �򋗗����ő�l�𒴂����ꍇ�͏C��
					if (JUMP_MAX < jumpPower) jumpPower = JUMP_MAX;
					// �W�����v�̔򋗗���o�^
					// ���̒l�͒n�ʂɐG�ꂽ or �V��ɐڐG�����ꍇ�A0�ɂ���B
					_gravity = -jumpPower;
				}
			}
		}
		_jumpPower = 0;
	}

	void Player::Rob(double x, double y) {
		if (_input != true) {
			_aState = ActionState::ROB;
			// �L�[��񂪈Ⴄ���A�L�[�����X�V
			if (_divKey.first != PKEY_ROB) {
				_divKey.first = PKEY_ROB;
				// SE�ǂݍ���
				auto sound1 = SoundResearch(key::SOUND_PLAYER_ROB);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));
#ifdef _DEBUG
				// �����蔻��̐ݒ�
				auto red = GetColor(255, 0, 0);
#endif
				// �����蔻��̐ݒ�i����C���j
				// �ȉ���no����
				//double minX, minY, maxX, maxY;
				//if (_direction) {
				//	// minX = minX - ROB_WIDTH;
				//	minX = x + PLAYER_WIDTH / 2;
				//	minY = y - PLAYER_HIGHT / 2;
				//	// maxX = (x + PLAYER_WIDTH / 2) + ROB_WIDTH;
				//	maxX = minX + ROB_WIDTH;
				//	maxY = y + PLAYER_HIGHT / 2;
				//}
				//else {
				//	minX = (x - PLAYER_WIDTH / 2) - ROB_WIDTH;
				//	minY = y - PLAYER_HIGHT / 2;
				//	maxX = minX + ROB_WIDTH;
				//	maxY = y + PLAYER_HIGHT / 2;
				//}

				//Vector2 amin(minX, minY);
				//Vector2 amax(maxX, maxY);

				auto it = _collisions.find(PKEY_ROB);
				// it->second.SetVector(amin, amax);
				it->second.GetbDrawFlg() = true;

			}
			_aCount = 0;
			_changeGraph = true;	// ��ԑJ�ڃt���O�I��
		}
	}

	void Player::Give(double x, double y) {

		if (_input != true) {
			_aState = ActionState::GIVE;
			if (_divKey.first != PKEY_GIVE) {
				_divKey.first = PKEY_GIVE;
				// SE�ǂݍ���
				auto sound1 = SoundResearch(key::SOUND_PLAYER_GIVE);
				PlaySoundMem(sound1, se::SoundServer::GetPlayType(_divKey.second));

				auto it = _collisions.find(PKEY_GIVE);
				it->second.GetbDrawFlg() = true;

			}
			_aCount = 0;
			_changeGraph = true;	// ��ԑJ�ڃt���O�I��
		}
	}

	void Player::ChangeSoul() {
	
	}

	void Player::Damage(){
	
	}

	bool Player::Dead() {
		return false; 
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
		Vector2 minV(dx - PLAYER_WIDTH / 2, dy + PLAYER_HIGHT / 2);
		Vector2 maxV(dx + PLAYER_WIDTH / 2, dy - PLAYER_HIGHT / 2);
		AABB playerAABB(minV, maxV);

		return playerAABB;
	}

	// �ʒu���W�X�V
	void Player::PositionUpdate() {
		// �ړ��x�N�g��Y�ɉ����x����
		_moveVector.GetPY() = _gravity;
		// �}�b�v�`�b�v�ɂ߂荞��ł���ꍇ�͍��W���C��
		_game.GetMapChips()->IsHit(_mainCollision, _position, _moveVector);
		_position = _position + _moveVector;	// �ʒu���W���X�V

		// �e�퓖���蔻��̍X�V
		auto it = _collisions.find(_divKey.first);
		if (it != _collisions.end()) {
			it->second.Update(_position, _direction);
		}

		if (_aState == ActionState::DASH) {
			_mainCollision.Swap(it->second);
		} else {
			_mainCollision.Update(_position, _direction);
		}
	}


	// �f�o�b�O�p�����i�ϐ��̕\���E�����蔻��̕`�擙�j
#ifdef _DEBUG
	void Player::DebugInfo() {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "ActionStatet : %d\n", _aState);
		DrawFormatString(0, 25, GetColor(255, 255, 255), "Animation : %d\n", _aCount);


		/*auto minX = box.GetMin().IntX();
		auto minY = box.GetMin().IntY();
		auto maxX = box.GetMax().IntX();
		auto maxY = box.GetMax().IntY();
		auto boxColor = GetColor(255, 255, 255);

		DrawBox(minX, minY, maxX, maxY, boxColor, FALSE);*/
		// DrawBox(box.GetMin().IntX(), box.GetMin().IntY(), box.GetMax().IntX(), box.GetMax().IntX(),
				// GetColor(255, 255, 255), FALSE);
	}
#endif
}