#include "TutorialServer.h"
#include "TutorialImage.h"

namespace inr {

	TutorialServer::TutorialServer() {
		Clear();
	}

	TutorialServer::~TutorialServer() {
		Clear();
	}

	bool TutorialServer::Clear() {
		_isActive = false;
		_images.clear();
		return true;
	}

	/*bool TutorialServer::Init() {
	}*/

	void TutorialServer::Add(std::unique_ptr<TutorialImage> timage) {
		_isActive = true;
		_images.emplace_back(std::move(timage));
	}

	void TutorialServer::Process() {
		if (_isActive == false) return;
		for (auto&& image : _images) {
			image->Process();
		}
	}

	void TutorialServer::Draw() {
		if (_isActive == false) return;
		for (auto&& image : _images) {
			image->Draw();
		}
	}
}
