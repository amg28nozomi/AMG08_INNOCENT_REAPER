#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "ObjectServer.h"
#include "SoundServer.h"
#include "Logo.h"
#include <memory>
#include <DxLib.h>

namespace inr {

	ModeTitle::ModeTitle(Game& game) : ModeBase(game) {
		_titleGh = std::make_unique<Logo>(_game.GetGame());
	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {
		// BGM���葱���Ă���ꍇ�͒�~����
		auto sound = se::SoundServer::GetSound(bgm::SOUND_TITLE);
		if (CheckSoundMem(sound) == TRUE) StopSoundMem(sound);	// BGM��~
		else {	// �Đ����Ă��Ȃ��ꍇ�͍Đ����J�n����
			PlaySoundMem(sound, se::SoundServer::GetPlayType(bgm::SOUND_TITLE));
		}
	}

	void ModeTitle::Process() {
			_titleGh->Process();
			_game.GetObjectServer()->Process();

		/*_titleGh->Process();
		_ui->Process();
		_game.GetObjectServer()->Process();*/
	}

	void ModeTitle::Draw() {
		_titleGh->Draw();
		_game.GetObjectServer()->Draw();
	}
}
