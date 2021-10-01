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
		_gType = GimmickType::DOOR;
		_color = -1;
		Init();
	}

	void Door::Init() {
		_switch = gimmick::OFF;
		_changeGraph = false;
		_ismove = false;
		_moves = {};
		_normalY = 0;

		_motionKey = {
			{ gimmick::door::KEY_DOOR_LEVER, {14 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BLUE, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_RED, {20 * 3, 0}},
			{ gimmick::door::KEY_DOOR_BOSS, {1, 0}},
		};
	}

	void Door::Process() {
		// if (_switch == gimmick::OFF && _ismove == false) return;	// �t���O���I���̏ꍇ�͏������I������
		if (DoorMove() == true) return;
		MotionCount();
	}

	void Door::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);

#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif

	}

	void Door::SetParameter(Vector2 spwan, std::string key, int flag) {
		_divKey.first = key;
		SetColor(key);
		// _motionKey = { { _divKey.first, {25, 50}} };
		_position = spwan;
		_ismove = false;

		auto ite = _motionKey.find(_divKey.first);

		bool colf;
		switch (flag) {
		case oscenario::gimmick::FLAG_FALSE:
			colf = true;	// �����蔻������ɖ߂�
			/*_position = spwan;*/
			_switch = gimmick::OFF;
			_aCount = 0;
			break;
		case oscenario::gimmick::FLAG_TRUE:
			/*_position = { spwan.GetX(), _normalY - OPEN_MAX };*/
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			_aCount = ite->second.first - 1;
			colf = false;	// �����蔻������ɖ߂�
			break;
		default:
			/*_position = spwan;*/
			_switch = gimmick::OFF;
			_aCount = 0;
			colf = true;
			break;
		}
		_mainCollision = { _position, 20, 20, 10, 70, colf };
	}

	void Door::SwitchOn() {
		_switch = gimmick::ON;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = false;
#endif
		_mainCollision.GetCollisionFlgB() = false;	// �����蔻������ɖ߂�
		auto sh = SoundResearch(gimmick::door::KEY_DOOR);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
	}

	void Door::SwitchOff() {
		_switch = gimmick::OFF;
		_ismove = true;
#ifdef _DEBUG
		_mainCollision.GetbDrawFlg() = true;
#endif
		_mainCollision.GetCollisionFlgB() = true;	// �����蔻������ɖ߂�
	}

	bool Door::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// �Ώۂ͐ڐG���Ă��邩�H
		if (_mainCollision.HitCheck(box) == false) return false;	// �Փ˂��Ă��Ȃ�


		if (move.GetX() < 0) {
			// ������ڐG���Ă���
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		}
		else if (0 < move.GetX()){
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;
	}

	bool Door::DoorMove() {
		// �{�X���ł͂Ȃ��ꍇ�͏����𒆒f����
		if (_divKey.first != gimmick::door::KEY_DOOR_BOSS) return false;
		if (_ismove == false) return true;
		// �t���O�ɉ����ď�����؂�ւ���
		switch (_switch) {
		case false:
			_moves.GetPY() = DOOR_VECTOR;
			_position = _position + _moves;
			if (_normalY <= _position.GetY()) {
				_position.GetPY() = _normalY;
				_ismove = false;
			}
			break;
		case true:
			_moves.GetPY() = -DOOR_VECTOR;
			_position = _position + _moves;
			if (_position.GetY() <= _normalY - OPEN_MAX) {
				_position.GetPY() = _normalY - OPEN_MAX;
				_ismove = false;
			}
			break;
		}
		_mainCollision.Update(_position, false);

		_moves = {};

	}

	bool Door::MotionCount() {
		if (_ismove == false) return false;
		switch (_switch) {
		case gimmick::ON:
			if (AnimationCountMax() == true) {
				if (_mainCollision.GetCollisionFlg() == true) {
					_mainCollision.GetCollisionFlgB() = false;
#ifdef _DEBUG
					_mainCollision.GetbDrawFlg() = false;
#endif
					_ismove = false;
				}
				return false;
			}
			++_aCount;
			return true;
		case gimmick::OFF:
			if (_aCount == 0) {
				if (_mainCollision.GetCollisionFlg() == false) {
					_mainCollision.GetCollisionFlgB() = true;
#ifdef _DEBUG
					_mainCollision.GetbDrawFlg() = true;
#endif
					_ismove = false;
				}
				return false;
			}
			--_aCount;
			return true;
		}
	}

	void Door::SetColor(std::string key) {
		// �F�w��
		//if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		if (key == gimmick::door::KEY_DOOR_RED) _color = static_cast<int>(soul::RED);
		else if (key == gimmick::door::KEY_DOOR_BLUE) _color = static_cast<int>(soul::BLUE);
		else _color = -1;
	}

	void Door::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// �I�u�W�F�N�g���̍X�V
		_divKey.first = gimmick::door::KEY_DOOR_BOSS;
		_normalY = _oValue.Positions().at(0).GetY();	// �܂��Ă���ۂ̕`����W

		bool colf;
		switch (_game.GetModeServer()->GetModeMain()->BossOpen()) {	// ���͊J����Ă��邩�H
		case true:	// �󂢂Ă���ꍇ
			colf = false;
			_position = { _oValue.Positions().at(0).GetX(), _normalY - OPEN_MAX };
			_switch = gimmick::ON;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = false;
#endif
			break;
		case false:	// ���Ă���ꍇ
			colf = true;	// �����蔻������ɖ߂�
			_position = _oValue.Positions().at(0);
			_switch = gimmick::OFF;
#ifdef _DEBUG
			_mainCollision.GetbDrawFlg() = true;
#endif
			break;
		}

		_mainCollision = { _position, 40, 60, 100, 130, colf };
	}
}