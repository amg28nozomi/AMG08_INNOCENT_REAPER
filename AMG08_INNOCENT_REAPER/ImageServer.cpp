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
		_images.clear();	// ‘S—v‘f‚Ì‰ğ•úˆ—‚ğs‚¤
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
		// ƒL[‚É•ÏX‚ª‰Á‚í‚Á‚Ä‚¢‚éê‡‚Ì‚İA•ÏXˆ—‚ğs‚¤
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
		return true;
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false; // ¸”s‚µ‚Ü‚µ‚½
		ite->second->Init();	// ‰Šú‰»ˆ—‚ğŒÄ‚Ño‚µ
		return true;
	}

	bool ImageServer::IsResister(const int number) {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return true;	// “o˜^ˆ—‚ğs‚¤
		return false;	// ‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚½‚ß“o˜^ˆ—‚ğs‚í‚È‚¢
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_changeKey != image::number::NUM) return false;
		_changeKey = nextKey;
		return true;
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// —v‘f‚Ì\¬
		return true;
	}
}