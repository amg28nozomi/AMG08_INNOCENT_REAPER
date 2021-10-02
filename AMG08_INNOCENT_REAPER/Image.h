#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	namespace background {

		constexpr auto BACK_GROUND_S = "back_groundS";
		constexpr auto STAGES_FILE = "bg_stageS_";
		constexpr auto STAGES_PATH = "Resource/BackGround/Stage0/";
		constexpr auto STAGES_HEIGHT = 2160;

		constexpr auto BACK_GROUND_1 = "back_ground1";
		constexpr auto STAGE1_FILE = "bg_stage1_";
		constexpr auto STAGE1_PATH = "Resource/BackGround/Stage1/";
		constexpr auto STAGE1_MAP_HEIGHT = 2160;

		constexpr auto BACK_GROUND_2 = "back_ground2";	// ステージ2
		constexpr auto STAGE2_FILE = "bg_stage2_";
		constexpr auto STAGE2_PATH = "Resource/BackGround/Stage2/";
		constexpr auto STAGE2_MAP_HEIGHT = 2160;

		constexpr auto BACK_GROUND_B = "back_groundB";
		constexpr auto STAGEB_FILE = "bg_stageb_";
		constexpr auto STAGEB_PATH = "Resource/BackGround/StageBoss/";
	}

	namespace end {
		constexpr auto STAFF_ROLL = "staff_roll";

		constexpr auto STAFF_ROLL_WIDTH = 1920;
		constexpr auto STAFF_ROLL_HEIGHT = 11880;
	}


	constexpr auto BACK_GROUND = "backP";
	constexpr auto BACK_GROUND_W = 1920;
	constexpr auto BACK_GROUND_H = 2160;

	constexpr auto TITLE_LOGO = "title_rogo";
	constexpr auto TITLE_IMAGE_W = 1920;
	constexpr auto TITLE_IMAGE_H = 1080;

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

