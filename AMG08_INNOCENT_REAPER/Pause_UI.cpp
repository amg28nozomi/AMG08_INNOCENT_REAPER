#include "Pause_UI.h"
#include "SoundServer.h"
#include <DxLib.h>

namespace {
	constexpr auto MOVE_NULL = 0;	// �ړ��ʖ���
	constexpr auto MOVE_UP = 1;
	constexpr auto MOVE_DOWN = -1;

	constexpr auto MOVE_VECTOR = 5.0;
}

namespace inr {

	Pause_UI::Pause_UI(Game& game) : Particle_Image(game) {
		_imageType = image::particle::UI;
		_no = 0;
		_upDown = MOVE_NULL;
		_nextY = 0;
	}

	void Pause_UI::Process() {
		Particle_Image::Process();
		// �ړ�����������ꍇ�́A�ڕW���W�����ֈړ�����
		if (_move == true) {
			// �ڕW�n�_�ւ̈ړ�������������
			if (_pos.GetY() == _nextY) {
				_upDown = MOVE_NULL;
				_nextY = MOVE_NULL;
				_move = false;	// �ړ������I��
				return;	// ������E�o����
			}
			_pos.GetPY() += (MOVE_VECTOR * _upDown);
		}
	}

	void Pause_UI::ChangePosition(bool type) {
		if (_move == true)	return;	// �ړ����������s���̏ꍇ�͏������s��Ȃ�Ȃ�
		switch (type) {
		case true:
			++_no;
			break;
		case false:
			--_no;
			break;
		}
		auto sound = se::SoundServer::GetSound(system::CURSOR_MOVE);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(system::CURSOR_MOVE));
		_move = true;

		switch (_no) {
		case -1:
			_nextY = 540;
			_no = 2;
			break;
		case 0:
			_nextY = 300;
			break;
		case 1:
			_nextY = 420;
			break;
		case 2:
			_nextY = 540;
			break;
		case 3:
			_no = 0;
			_nextY = 300;
			break;
		default:
			break;
		}
		if (_pos.GetY() - _nextY < 0) _upDown = MOVE_UP;	// �ړ������ɉ����āA�l��؂�ւ���
		else _upDown = MOVE_DOWN;
	}
}