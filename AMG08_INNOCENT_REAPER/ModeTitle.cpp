/*****************************************************************//**
 * @file   ModeTitle.cpp
 * @brief  タイトルを管理するモードタイトルクラス（モードベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ModeTitle.h"
#include "Game.h"
#include "ModeServer.h"
#include "ObjectServer.h"
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
		BgmManage();
		_titleGh->Init();
	}

	void ModeTitle::Process() {
		_titleGh->Process();
		_game.GetObjectServer()->Process();
	}

	void ModeTitle::Draw() {
		_titleGh->Draw();
		_game.GetObjectServer()->Draw();
	}
}
