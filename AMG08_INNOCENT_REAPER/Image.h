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

	constexpr auto AMG_LOGO = "amg";	// グッバイ
	constexpr auto AMG_IMAGE = 400;

	constexpr auto TEAM_LOGO = "team_logo";	// teamロゴ
	constexpr auto TEAM_IMAGE = 140;

	class Game;

	class Image {
	protected:
		Game& _game;
		Vector2 _pos;

		std::string _graphKey;	// グラフィックハンドル取得用のキー

	public:
		Image(Game& game);
		~Image() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();
	};
}

