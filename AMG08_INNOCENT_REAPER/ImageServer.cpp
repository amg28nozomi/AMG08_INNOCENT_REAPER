/*****************************************************************//**
 * @file   ImageServer.cpp
 * @brief  �C���[�W�T�[�o�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ImageServer.h"
#include "Particle_Image.h"
#include "Game.h"

namespace inr {

	ImageServer::ImageServer(Game& game) : _game(game) {
		Init();
	}

	ImageServer::~ImageServer() {
		Init();
	}

	bool ImageServer::Init() {
		// �e�평����
		ImageClear();
		_imageKey = image::number::NUM;
		_changeKey = image::number::NUM;
		_active = false;
		_input = false;
		return true;
	}

	bool ImageServer::ImageClear() {
		_images.clear();	// �S�v�f�̉���������s��
		return true;
	}

	bool ImageServer::Process() {
		auto ite = _images.find(_imageKey);				// �L�[�̌���
		if (ite == _images.end()) return false;		// �q�b�g���Ȃ��ꍇ�͏������I��
		// �`��t���O���Ȃ��ꍇ
		if (ite->second->IsDraw() != true) {
			_active = false;			// �񊈐���ԂɑJ��
			_input = false;				// ���͂��󂯕t���Ȃ�
			return false;					// �X�V�I��
		}
		// �摜�̃A�j���[�V�������s���Ă��Ȃ���Ԃ��A���͏�Ԃ��I�t�̏ꍇ
		if (_input != true && ite->second->IsNormal() == true) {
			_input = true;				// ���͏�ԃI��
		}
		ite->second->Process();	// ���݂̃L�[�̏��������s����
		Input();								// ���͏����Ăяo��
		return true;						// �X�V����
	}

	bool ImageServer::Draw() {
		auto ite = _images.find(_imageKey);				// �摜�̎��o��
		if (ite == _images.end()) return false;		// ���o���Ɏ��s�����ꍇ�͏������f
		ite->second->Draw();											// �`�揈���Ăяo��
		return true;															// �`�搬��
	}

	bool ImageServer::ChangeKey() {
		// �L�[�ɕύX��������Ă���ꍇ�̂݁A�ύX�������s��
		if (_changeKey == image::number::NUM) return false;
		ImageInit();											// �摜�̏�����
		_imageKey = _changeKey;						// �L�[�̐؂�ւ�
		_changeKey = image::number::NUM;	// �؂�ւ��p�L�[����ɂ���
		_active = true;										// ������Ԃɐ؂�ւ�
		return true;											// �؂�ւ�����
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);				// �摜�̎��o��
		if (ite == _images.end()) return false;		// ���o�����s
		ite->second->Init();											// �����������Ăяo��
		return true;															// ����������
	}

	bool ImageServer::IsResister(const int number) {
		auto ite = _images.find(_imageKey);				// �摜�̎��o��
		if (ite == _images.end()) return true;		// �o�^�������s��
		return false;															// ���łɓo�^����Ă��邽�ߓo�^�������s��Ȃ�
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_active == true) return false;				// ������Ԃ̏ꍇ�͒��f
		if (_changeKey == nextKey) return false;	// �L�[�������ꍇ�͒��f
		_changeKey = nextKey;											// �L�[�̍X�V
		return true;															// �؂�ւ�����
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// �R���e�i�ɓo�^
		return true;	// �����I��
	}

	bool ImageServer::Input() {
		if (_input != true) return false;											// ���͏������󂯕t���Ă��Ȃ�
		auto ite = _images.find(_imageKey);										// �摜�̎��o��
		if (ite->second->IsNormal() != true) return false;		// �ʏ��Ԃł͂Ȃ��ꍇ�͏������s��Ȃ�
		if (_game.GetTrgKey() != PAD_INPUT_4) return false;		// B�{�^���͉�����Ă��Ȃ�
		ite->second->End();																		// �����ꂽ�ꍇ�͏I�������Ăяo��
		_input = false;																				// ���͏�Ԃ��I�t�ɂ���
		return true;																					// ��������
	}

	bool ImageServer::IsActive() {
		if (_active == true) return true;											// ���ɋ@�\���Ă���
		if (_changeKey == image::number::NUM) return false;		// �@�\���Ă��Ȃ�
		ImageInit();																					// ���̉摜�̏����������Ăяo��
		_imageKey = _changeKey;																// �L�[�̍X�V
		_changeKey = image::number::NUM;											// �؂�ւ��p�L�[����ɂ���
		_active = true;																				// �����J�n
		// �`����J�n����
		auto ite = _images.find(_imageKey);
		ite->second->DrawStart();
		return true;
	}
}