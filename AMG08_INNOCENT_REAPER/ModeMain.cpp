#include "ModeMain.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include <memory>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_stageKey = "null";
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// �e��I�u�W�F�N�g���T�[�o�ɓo�^����
			// �I�u�W�F�N�g�T�[�o�Ƀv���C���[��o�^
			auto player = std::make_unique<Player>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(player));
			auto enemy = std::make_unique<SoldierDoll>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(enemy));
			_bg = std::make_unique<BackGround>();
			TimeClear();
		}

		_worldPosition = { 1920 / 2, 1080 / 2 };
	}

	void ModeMain::Process() {
		++_modeFrame;
		// ���[���h���W�X�V
		_worldPosition = _game.GetObjectServer()->GetPlayer().GetPosition();

		_game.GetMapChips()->Process();
		_game.GetObjectServer()->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
	}

	bool ModeMain::Clamp(Vector2& pos) {
		auto x = pos.GetX() - _worldPosition.GetX();
		auto y = pos.GetY() - _worldPosition.GetY();
		// �Ώۂ̍��W����J�������W���������l�̓v���X���ǂ����H
		if (x < 0 && 0 < y) {
			// �X�N���[�����W���ɑ��݂��Ȃ�
			return false;
		}
		// �Ώۂ͌��݂̃X�N���[�����W��ɑ��݂��Ă��邩�H
		if( WINDOW_MIN <= x <= WINDOW_W && WINDOW_MIN <= y <= WINDOW_H) {
			// ���[���h���W����X�N���[�����W�ɏC��
			pos.GetPX() = x;
			pos.GetPY() = y;
			return true;
		}
		return false;
	}
}
