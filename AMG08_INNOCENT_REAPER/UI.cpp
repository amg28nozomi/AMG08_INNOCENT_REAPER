#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include "SoulSkin.h"
#include <queue>

namespace {
	constexpr auto UI_SPACE_W = 100;
}

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = _game.GetObjectServer()->GetPlayer();
		_pos = { 300, 200 };
		_ghKeys = { ui::KEY_HP };
		_count = { 0 };
		Init();
	}

	void UI::Init() {
		// �e�폈���̏�����
	}

	void UI::Process() {

		auto ps = _player->GetSouls();
		if (ps.empty()) {
			// ������̏ꍇ�͉����s��Ȃ���
		}


		// �X�V����
		// ���@���ۗL���鍰�̐����ς�����ꍇ�̂ݍX�V��������
		// �Y�����鍰���Ȃ��Ȃ����ꍇ�A�j���[�V�������J�n����
	}

	void UI::Draw() {
		for (auto number = 0; number < _player->GetSouls().size() + 1; ++number) {
			DrawRotaGraph(_pos.IntX() * number, _pos.IntY(), 1.0, 0, )
			// ����̂ݒE�o
			if (number == 0) continue;
		}
	}
}
