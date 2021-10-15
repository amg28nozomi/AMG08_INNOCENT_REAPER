#pragma once
#include <memory>
#include <vector>

namespace inr {

	class TutorialImage;

	class TutorialServer {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 */
		TutorialServer();
		/**
		 * @brief	�f�X�g���N�^
		 */
		~TutorialServer();

		/*bool Init(const std::string key);*/
		void Process();
		void Draw();

		bool Clear();

		void Add(std::unique_ptr<TutorialImage> timage);
	private:
		bool _isActive;
		std::vector<std::unique_ptr<TutorialImage>> _images;
	};
}

