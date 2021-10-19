/*****************************************************************//**
 * @file   ModeServer.cpp
 * @brief  �e�탂�[�h���Ǘ����郂�[�h�T�[�o
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeTitle.h"
#include "ModeMain.h"
#include "ModeEnd.h"
#include "Game.h"
#include "ObjectServer.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace {
	constexpr auto MODE_NULL = "null";
	constexpr auto CHANGE_INTERVAL = 60;
}

namespace inr {

	ModeServer::ModeServer(Game& game) : _game(game) {
		Init();
	}

	ModeServer::~ModeServer() {
		ClearModeLists();
	}

	void ModeServer::Init() {
		// �A�z�z�񏉊���
		ClearModeLists();
		// �e�탂�[�h�o�^
		_modes.emplace(mode::TITLE, std::make_shared<ModeTitle>(_game.GetGame()));	// �^�C�g��
		// �Z���N�g�i�������j
		_modes.emplace(mode::MAIN, std::make_shared<ModeMain>(_game.GetGame()));	// �Q�[���{��
		_modes.emplace(mode::FIN, std::make_shared<ModeEnd>(_game.GetGame()));	// �v���O�����I���O�̏���
		_fadeBlack = std::make_unique<FadeBlack>(_game.GetGame());
		// �����L�[��TitleMode�ɐݒ�
#ifdef _DEBUG
		// _modeKey = mode::MAIN;
		_modeKey = mode::TITLE;
#endif
#ifndef _DEBUG
		_modeKey = mode::TITLE;
#endif
		_ChangeKey = MODE_NULL;
		ModeInit();
	}

	void ModeServer::Process() {
		// Mode�̍X�V�p�L�[���ύX����Ă��邩�ǂ���
		IsModeChange();
		// ���ݐݒ肳��Ă��郂�[�h�����o���A�X�V���������s�B
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Process();
		_fadeBlack->Process();
	}

	void ModeServer::Draw() {
		// ���݂̃��[�h�����o���A�`�揈�������s�B
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Draw();
		_fadeBlack->Draw();
	}

	void ModeServer::ClearModeLists() {
		_modes.clear();
	}

	void ModeServer::ModeChange(std::string nextMode, int interval) {
		_ChangeKey = nextMode;	// ���[�h�؂�ւ��t���O�I��

		if (interval == 0) interval = CHANGE_INTERVAL;
		_fadeBlack->FlagChange(image::FADE_OUT, interval);
	}

	bool ModeServer::ModeInit() {
		auto mode = _modes.find(_modeKey);				// ���݂̃��[�h�����o��
		if (mode == _modes.end()) return false;		// �q�b�g���Ȃ������i���������s�j
		mode->second->Init();											// �������������s
		return true;
	}

	void ModeServer::IsModeChange() {
		if (_ChangeKey == MODE_NULL) return;		// �t���O���I�t�̏ꍇ�͔�����
		// �������؂�ւ�����u�ԂɈ�x�����I�u�W�F�N�g�̍Ĕz�u�����s��
		if (_fadeBlack->PalChange() == true) {
			ModeInit();														// ���݂̃��[�h��������
			_game.GetObjectServer()->AllClear();
			_modeKey = _ChangeKey;								// �X�V�������Ă���ꍇ�͎��s�p�L�[���X�V
			ModeInit();														// ���̃��[�h��������
			_ChangeKey = MODE_NULL;								// �X�V�p�L�[��������
		}
	}

	void ModeServer::GameEnd() {
		// �Q�[�����I������
		_fadeBlack->FlagChange(image::FADE_OUT, CHANGE_INTERVAL);
		_game.ProgramEnd();
	}

	std::shared_ptr<ModeMain> ModeServer::GetModeMain() {
		auto mode = _modes.find(mode::MAIN);
		return std::dynamic_pointer_cast<ModeMain>(mode->second);
	}
}