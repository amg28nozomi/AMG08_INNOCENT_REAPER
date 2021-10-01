#include "ModeEnd.h"
#include "Game.h"
#include "ModeServer.h"

namespace inr {

	ModeEnd::ModeEnd(Game& game) : ModeBase(game), _end(false) {
	}

	void ModeEnd::Init() {
		BgmManage();	// BGM�̐؂�ւ�
	}

	void ModeEnd::Process() {
		if (IsEnd() == true) return;
		// �ړ��������s��
	}

	void ModeEnd::Draw() {

	}

	bool ModeEnd::IsEnd() {
		if (_end != true) return false;
		_game.GetModeServer()->ModeChange(mode::TITLE);	// �^�C�g���ɑJ�ڂ���
		return true;
	}
}
