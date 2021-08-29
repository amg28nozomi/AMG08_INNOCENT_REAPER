#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeTitle.h"
#include "ModeMain.h"
#include "ModeEnd.h"
#include "Game.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace {
	constexpr auto MODE_NULL = "null";
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
		_modes.emplace(mode::TITLE, std::make_unique<ModeTitle>(_game.GetGame()));	// �^�C�g��
		// �Z���N�g�i�������j
		_modes.emplace(mode::MAIN, std::make_unique<ModeMain>(_game.GetGame()));	// �Q�[���{��
		_modes.emplace(mode::FIN, std::make_unique<ModeEnd>(_game.GetGame()));	// �v���O�����I���O�̏���
		
		// �����L�[��TitleMode�ɐݒ�
		_modeKey = mode::MAIN;

		// _modeKey = mode::TITLE;
		_ChangeKey = MODE_NULL;
		ModeInit();
	}

	void ModeServer::Process() {
		// Mode�̍X�V�p�L�[���ύX����Ă��邩�ǂ���
		if (_ChangeKey != MODE_NULL) {
			// ���݂̃��[�h��������
			ModeInit();
			// �X�V�������Ă���ꍇ�͎��s�p�L�[���X�V
			_modeKey = _ChangeKey;
			// ���̃��[�h��������
			ModeInit();
			// �X�V�p�L�[��������
			_ChangeKey = MODE_NULL;
		}

		// ���ݐݒ肳��Ă��郂�[�h�����o���A�X�V���������s�B
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Process();
	}

	void ModeServer::Draw() {
		// ���݂̃��[�h�����o���A�`�揈�������s�B
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Draw();
	}

	void ModeServer::ClearModeLists() {
		_modes.clear();
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

	std::unique_ptr<ModeBase>& ModeServer::GetMode(std::string modeKey) {
		auto mode = _modes.find(modeKey);
		return mode->second;
	}
}