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
		_images.clear();	// 全要素の解放処理を行う
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
		// キーに変更が加わっている場合のみ、変更処理を行う
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
		return true;
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false; // 失敗しました
		ite->second->Init();	// 初期化処理を呼び出し
		return true;
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_changeKey != image::number::NUM) return false;
		_changeKey = nextKey;
		return true;
	}
}