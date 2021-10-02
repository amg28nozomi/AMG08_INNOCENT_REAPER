#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

namespace inr {

	class TutorialImage;

	using TutorialMap = std::unordered_map<std::string, std::vector<ImageValue>>;

	class TutorialServer {
	public:
		TutorialServer();
		~TutorialServer();

		bool Init();
		void Process();
		void Draw();

		bool Clear();
	private:
		bool _isActive;
		std::string _skey;	// 現在のステージ
		std::unordered_map<std::string, std::vector<std::unique_ptr<TutorialImage>>> _images;
	};
}

