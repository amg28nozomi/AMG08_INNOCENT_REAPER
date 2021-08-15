#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	constexpr auto BACK_GROUND = "backP";
	constexpr auto BACK_GROUND_W = 1920;
	constexpr auto BACK_GROUND_H = 2160;

	constexpr auto TITLE_LOGO = "title_rogo";
	constexpr auto TITLE_IMAGE_W = 1920;
	constexpr auto TITLE_IMAGE_H = 1080;

	class Game;

	class Image {
	protected:
		Game& _game;
		Vector2 _pos;

		std::string _graphKey;	// �O���t�B�b�N�n���h���擾�p�̃L�[

	public:
		Image(Game& game);
		~Image() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();
	};
}
