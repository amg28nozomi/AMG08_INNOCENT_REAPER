#include "TutorialServer.h"
#include "TutorialImage.h"
#include "Loads.h"

namespace inr {

	TutorialServer::TutorialServer() {
		Init();
	}

	TutorialServer::~TutorialServer() {
		Init();
	}

	bool TutorialServer::Clear() {
		_isActive = false;
		_images.clear();
	}

	bool TutorialServer::Init() {
		if (_images.empty() != true) return false;	// —v‘f‚ª‚ ‚éê‡‚Íˆ—‚ðs‚í‚È‚¢
		// ‰æ‘œ“Ç‚Ýž‚Ý


	}

	void TutorialServer::Process() {
		if (_isActive == false) return;
		auto images = _images.find(_skey);
		for (auto&& image : images->second) {
			image->Draw();
		}
	}

	void TutorialServer::Draw() {
		if (_isActive == false) return;
		auto images = _images.find(_skey);
		for (auto&& image : images->second) {
			image->Draw();
		}
	}
}
