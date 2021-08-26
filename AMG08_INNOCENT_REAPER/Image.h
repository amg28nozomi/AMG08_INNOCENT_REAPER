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

	constexpr auto TITLE_BG = "title_back_ground";
	constexpr auto TITLE_IMAGE_WIDTH = 1920;
	constexpr auto TITLE_IMAGE_HEIGHT = 1080;

	// 各種UI
	constexpr auto TITLE_START1 = "ui_start1";
	constexpr auto TITLE_START2 = "ui_start2";
	constexpr auto TITLE_EXIT1 = "ui_exit1";
	constexpr auto TITLE_EXIT2 = "ui_exit2";

	constexpr auto TITLE_UI_HEIGHT = 166;
	constexpr auto TITLE_START_WIDTH = 372;
	constexpr auto TITLE_EXIT_WIDTH = 298;

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

		virtual void FlagOn();
	};
}

