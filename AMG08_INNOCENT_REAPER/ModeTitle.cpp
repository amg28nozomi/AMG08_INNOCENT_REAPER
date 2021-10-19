/*****************************************************************//**
 * @file   ModeTitle.cpp
 * @brief  �^�C�g�����Ǘ����郂�[�h�^�C�g���N���X�i���[�h�x�[�X�̃T�u�N���X�j
 *
 * @author ��؊�C
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
		// BGM���葱���Ă���ꍇ�͒�~����
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
