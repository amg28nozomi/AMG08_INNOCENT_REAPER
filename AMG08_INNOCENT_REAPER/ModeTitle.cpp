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
		_bgmKey = bgm::SOUND_TITLE;
	}

	ModeTitle::~ModeTitle() {

	}

	void ModeTitle::Init() {
		// BGMが鳴り続けている場合は停止する
		auto sound = se::SoundServer::GetSound(_bgmKey);
		if (CheckSoundMem(sound) == TRUE) StopSoundMem(sound);	// BGM停止
		else {	// 再生していない場合は再生を開始する
			PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
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
