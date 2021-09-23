#include "ImageServer.h"
#include "Particle_Image.h"

namespace inr {

	ImageServer::ImageServer() {
		Init();
	}

	ImageServer::~ImageServer() {
		Init();
	}

	bool ImageServer::Init() {
		ImageClear();
		_imageKey = image::number::NUM;
		_changeKey = image::number::NUM;
		return true;
	}

	bool ImageServer::ImageClear() {
		_images.clear();	// �S�v�f�̉���������s��
		return true;
	}

	bool ImageServer::Process() {
		ChangeKey();

		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;
		ite->second->Process();
	}

	bool ImageServer::Draw() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;
		ite->second->Draw();
	}

	bool ImageServer::ChangeKey() {
		// �L�[�ɕύX��������Ă���ꍇ�̂݁A�ύX�������s��
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
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
		if (_changeKey != image::number::NUM) return false;
		_changeKey = nextKey;
		return true;
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// �v�f�̍\��
		return true;
	}
}