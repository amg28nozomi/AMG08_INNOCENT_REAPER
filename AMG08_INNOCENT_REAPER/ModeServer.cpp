#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeTitle.h"
#include "ModeMain.h"
#include "ModeEnd.h"
#include "Game.h"
#include <memory>
#include <unordered_map>
#include <string>

#include "ObjectServer.h"

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

	void ModeServer::ModeChange(std::string nextMode) {
		_ChangeKey = nextMode;	// ���[�h�؂�ւ��t���O�I��
		_fadeBlack->FlagChange(image::FADE_OUT, CHANGE_INTERVAL);
	}

	bool ModeServer::ModeInit() {
		// ���݂̃��[�h�����o��
		auto mode = _modes.find(_modeKey);
		// �q�b�g���Ȃ������i���������s�j
		if (mode == _modes.end()) return false;
		// �������������s
		mode->second->Init();
		return true;
	}

	void ModeServer::IsModeChange() {
		// �t���O���I�t�̏ꍇ�͔�����
		if (_ChangeKey == MODE_NULL) return;
		// �������؂�ւ�����u�ԂɈ�x�����A�I�u�W�F�N�g�̍Ĕz�u�����s��
		if (_fadeBlack->PalChange() == true) {
			// ���݂̃��[�h��������
			ModeInit();
			_game.GetObjectServer()->AllClear();
			// �X�V�������Ă���ꍇ�͎��s�p�L�[���X�V
			_modeKey = _ChangeKey;
			// ���̃��[�h��������
			ModeInit();
			// �X�V�p�L�[��������
			_ChangeKey = MODE_NULL;
		}
	}

	bool ModeServer::IsFadeChange() { 
		return _fadeBlack->PalChange();
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