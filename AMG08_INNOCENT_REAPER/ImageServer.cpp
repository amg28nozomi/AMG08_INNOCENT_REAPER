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

		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;
		if (ite->second->IsDraw() != true) {
			// ite->second->Init();
			_active = false;
			_input = false;
			return false;
		}
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
		if (_active == true) return true;
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
		_active = true;

		auto ite = _images.find(_imageKey);
		ite->second->DrawStart();
		return true;
	}
}