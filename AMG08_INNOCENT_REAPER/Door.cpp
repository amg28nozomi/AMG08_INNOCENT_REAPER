#include "Door.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "SoulSkin.h"

#include "ModeServer.h"
#include "ModeMain.h"

namespace {
	constexpr auto OPEN_MAX = 200;
	constexpr auto ANIMATION_FRAME = 50;
	constexpr auto DOOR_VECTOR = OPEN_MAX / ANIMATION_FRAME;
}

namespace inr {

	Door::Door(Game& game) : GimmickBase(game) {
		// ������
		_gType = GimmickType::DOOR;
		_color = -1;
		Init();
	}
	// ������
	void Door::Init() {
		// �e�평����
		_switch = gimmick::OFF;
		_changeGraph = false;
		_ismove = false;
		_moves = {};
		_normalY = 0;
		// �e��L�[�̐ݒ�
		_motionKey = {
			{ gimmick::door::KEY_DOOR_LEVER, {14 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BLUE, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_RED, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BOSS, {1, 0}},
		};
	}
	// �X�V
	void Door::Process() {
		// �h�A�̈ړ�����������ꍇ�͒��f
		if (DoorMove() == true) return;
		MotionCount();	// �A�j���[�V�����̍Đ�
	}
	// �`��
	void Door::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		// �����蔻��̕`��
		DrawDebugBox(_mainCollision);
#endif
	}
	// �I�u�W�F�N�g���̐ݒ�
	void Door::SetParameter(Vector2 spwan, std::string key, int flag) {
		// �e��o�^
		_divKey.first = key;
		SetColor(key);	// �F�̔���
		_position = spwan;
		_ismove = false;
		// �Ή�������̓o�^���Ăяo��
		auto ite = _motionKey.find(_divKey.first);

		bool colf;	// �����蔻��̃t���O
		// �M�~�b�N�t���O�ɉ����āA��Ԑ؂�ւ�
		switch (flag) {
		case oscenario::gimmick::FLAG_FALSE:
			colf = true;	// �����蔻������ɖ߂�
			_switch = gimmick::OFF;
			_aCount = 0;
			break;
		case oscenario::gimmick::FLAG_TRUE:
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			_aCount = ite->second.first - 1;
			colf = false;	// �����蔻������ɖ߂�
			break;
		default:
			_switch = gimmick::OFF;
			_aCount = 0;
			colf = true;
			break;
		}
		// �����蔻��̏C��
		_mainCollision = { _position, 20, 20, 10, 70, colf };
	}
	// �J�t���O�̋N��
	void Door::SwitchOn() {
		_switch = gimmick::ON;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
		_mainCollision.SetCollisionFlag() = false;	// �����蔻������ɖ߂�
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}
	// �J�t���O�̗}��
	void Door::SwitchOff() {
		auto sound = se::SoundServer::GetSound(gimmick::door::SE_CLOSE_DOOR);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));

		_switch = gimmick::OFF;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = true;
#endif
		_mainCollision.SetCollisionFlag() = true;	// �����蔻������ɖ߂�
	}
	// �����o������
	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// �Ώۂ͐ڐG���Ă��邩�H
		if (_mainCollision.HitCheck(box) == false) return false;	// �Փ˂��Ă��Ȃ�
		// �ڐG���Ă���ꍇ�A�ړ������Ƃ͋t�����ɉ����o��
		if (move.GetX() < 0) {
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		} else if (0 < move.GetX()){
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;	// �Փ˂��Ă���
	}
	// �h�A�̈ړ�����
	bool Door::DoorMove() {
		// �{�X���ł͂Ȃ��ꍇ�͏����𒆒f����
		if (_divKey.first != gimmick::door::KEY_DOOR_BOSS) return false;
		if (_ismove == false) return true;	// �ړ��t���O�������Ă��Ȃ��ꍇ�����f
		// �t���O�ɉ����ď�����؂�ւ���
		switch (_switch) {
			// ����߂�
		case false:
			_moves.GetPY() = DOOR_VECTOR;
			_position = _position + _moves;
			if (_normalY <= _position.GetY()) {
				_position.GetPY() = _normalY;
				_ismove = false;
			}
			break;
			// �����J����
		case true:
			_moves.GetPY() = -DOOR_VECTOR;
			_position = _position + _moves;
			if (_position.GetY() <= _normalY - OPEN_MAX) {
				_position.GetPY() = _normalY - OPEN_MAX;
				_ismove = false;
			}
			break;
		}
		// �e��X�V
		_mainCollision.Update(_position, false);
		_moves = {};
	}
	// �A�j���[�V��������
	bool Door::MotionCount() {
		// �ړ��t���O�������Ă���ꍇ�̂ݍĐ����s��
		if (_ismove == false) return false;
		// ���݂̊J�t���O�ɉ����ăA�j���[�V�������@�ύX
		switch (_switch) {
		case gimmick::ON:
			if (AnimationCountMax() == true) {
				// �A�j���[�V�����J�E���^������ɓ��B�����ꍇ�A�e�픻����U�ɂ���
				if (_mainCollision.GetCollisionFlag() == true) {
					_mainCollision.SetCollisionFlag() = false;
#ifdef _DEBUG
					_mainCollision.SetDrawFlag() = false;
#endif
					_ismove = false;
				}
				return false;
			}
			++_aCount;
			return true;
		case gimmick::OFF:
			if (_aCount == 0) {
				// �A�j���[�V�����J�E���^�������ɓ��B�����ꍇ�A�e�픻���^�ɂ���
				if (_mainCollision.GetCollisionFlag() == false) {
					_mainCollision.SetCollisionFlag() = true;
#ifdef _DEBUG
					_mainCollision.SetDrawFlag() = true;
#endif
					_ismove = false;
				}
				return false;
			}
			--_aCount;
			return true;
		}
	}
	// �F�̔���
	void Door::SetColor(std::string key) {
		// �L�[�ɉ����ĐF��؂�ւ���
		if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		else if (key == gimmick::door::KEY_DOOR_BLUE) _color = static_cast<int>(soul::BLUE);
		else _color = -1;
	}
	// �I�u�W�F�N�g���̓o�^
	void Door::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// �I�u�W�F�N�g���̍X�V
		_divKey.first = gimmick::door::KEY_DOOR_BOSS;	// �摜�L�[�̐ݒ�
		_normalY = _oValue.Positions().at(0).GetY();	// �܂��Ă���ۂ̕`����W

		bool colf = false;
		switch (_game.GetModeServer()->GetModeMain()->BossOpen()) {	// ���͊J����Ă��邩�H
		case true:	// �󂢂Ă���ꍇ
			colf = false;
			_position = { _oValue.Positions().at(0).GetX(), _normalY - OPEN_MAX };
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			break;
		case false:	// ���Ă���ꍇ
			colf = true;	// �����蔻������ɖ߂�
			_position = _oValue.Positions().at(0);
			_switch = gimmick::OFF;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = true;
#endif
			break;
		}
		// �����蔻��̏C��
		_mainCollision = { _position, 40, 60, 100, 130, colf };
	}
}