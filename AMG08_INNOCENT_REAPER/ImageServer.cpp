/*****************************************************************//**
 * \file   ImageServer.cpp
 * \brief  �C���[�W�T�[�o�N���X
 *		   �A�C�e���e�L�X�g���Ǘ�����
 * 
 * \author nozom
 * \date   October 2021
 *********************************************************************/
#include "ImageServer.h"
#include "Particle_Image.h"
#include "Game.h"

namespace inr {
	// �R���X�g���N�^
	ImageServer::ImageServer(Game& game) : _game(game) {
		Init();		// ������
	}
	// �f�X�g���N�^
	ImageServer::~ImageServer() {
		Init();		// ������
	}
	// ������
	bool ImageServer::Init() {
		// �e�평����
		ImageClear();
		_imageKey = image::number::NUM;
		_changeKey = image::number::NUM;
		_active = false;
		_input = false;
		return true;
	}
	// �R���e�i�̉��
	bool ImageServer::ImageClear() {
		_images.clear();	// �S�v�f�̉���������s��
		return true;
	}
	// �X�V
	bool ImageServer::Process() {
		// �L�[�̌���
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;		// �q�b�g���Ȃ��ꍇ�͏����������I��
		// �`��t���O���Ȃ��ꍇ
		if (ite->second->IsDraw() != true) {
			_active = false;	// �񊈐���ԂɑJ��
			_input = false;		// ���͂��󂯕t���Ȃ�
			return false;
		}
		// 
		if (_input != true && ite->second->IsNormal() == true) {
			_input = true;
		}
		ite->second->Process();	// ���݂̃L�[�̏��������s����
		Input();
		return true;
	}

	bool ImageServer::Draw() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;
		ite->second->Draw();
		return true;
	}

	bool ImageServer::ChangeKey() {
		// �L�[�ɕύX��������Ă���ꍇ�̂݁A�ύX�������s��
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
		_active = true;
		return true;
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false; // ���s���܂���
		ite->second->Init();	// �������������Ăяo��
		return true;
	}

	bool ImageServer::IsResister(const int number) {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return true;	// �o�^�������s��
		return false;	// ���łɓo�^����Ă��邽�ߓo�^�������s��Ȃ�
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_active == true) return false;
		if (_changeKey == nextKey) return false;
		_changeKey = nextKey;
		return true;
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// �v�f�̍\��
		return true;
	}

	bool ImageServer::Input() {
		if (_input != true) return false;
		auto ite = _images.find(_imageKey);
		if (ite->second->IsNormal() != true) return false;
		if (_game.GetTrgKey() != PAD_INPUT_4) return false;
		ite->second->End();
		_input = false;
		return true;
	}

	bool ImageServer::IsActive() {
		if (_active == true) return true;					// ���ɋ@�\���Ă���
		if (_changeKey == image::number::NUM) return false;	// �@�\���Ă��Ȃ�
		ImageInit();										// ���̉摜�̏����������Ăяo��
		_imageKey = _changeKey;								// �L�[�̍X�V
		_changeKey = image::number::NUM;					// �؂�ւ��p�L�[����ɂ���
		_active = true;										// �����J�n
		// �`����J�n����
		auto ite = _images.find(_imageKey);
		ite->second->DrawStart();
		return true;
	}
}