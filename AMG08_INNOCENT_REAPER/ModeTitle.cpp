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
		// BGM‚ª–Â‚è‘±‚¯‚Ä‚¢‚éê‡‚Í’âŽ~‚·‚é
		auto sound = se::SoundServer::GetSound(bgm::SOUND_TITLE);
		if (CheckSoundMem(sound) == TRUE) StopSoundMem(sound);	// BGM’âŽ~
		else {	// Ä¶‚µ‚Ä‚¢‚È‚¢ê‡‚ÍÄ¶‚ðŠJŽn‚·‚é
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
