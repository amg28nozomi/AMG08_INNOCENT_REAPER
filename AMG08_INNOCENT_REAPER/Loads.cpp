#include "Loads.h"
#include "ResourceServer.h"
#include "Game.h"
#include "Scenario.h"

#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "BigDoll.h"
#include "SoulSkin.h"
#include "EffectBase.h"
#include "Image.h"
#include "Logo.h"
#include "FadeBlack.h"
#include "UI.h"
#include "SoulCursor.h"
#include "Lever.h"
#include "GimmickBase.h"

#include <vector>

namespace {
	constexpr auto NUM_SIMPLE = 1;

	// 生成座標
	namespace stage_1 {
		constexpr auto START_POS_X = 8000;
		constexpr auto START_POS_Y = 1700;
	}
}

namespace inr {

	const graph::ResourceServer::DivGraphMap div{
		{PKEY_IDOL , {"Resource/Player/r_stand.png", 7, 3, 15, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_RUN, {"Resource/Player/r_move.png", 7, 2, 9, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_DASH,{"Resource/Player/r_dash.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_ROB , {"Resource/Player/r_rob.png", 7, 2, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_GIVE, {"Resource/Player/r_give.png", 7, 3, 16, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_JUMP, {"Resource/Player/r_jump.png", 7, 1, 6, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_FALL, {"Resource/Player/r_fall.png", 7, 2, 10, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_HIT, {"Resource/Player/r_hit.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
	};

	// ソルジャードールの各種モーション
	const graph::ResourceServer::DivGraphMap enemys {
		// 魂が空の状態
		{ enemy::SOLDIER_EMPTY, {"Resource/SoldierDoll/sd_e_down.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::BIG_EMPTY, {"Resource/BigDoll/bd_e_down.png", 7, 1, 7, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		// 赤い魂の状態
		{ enemy::red::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_r_wakeup.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_r_stand.png", 8, 1, 8, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_r_move.png", 9, 1, 9, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_ATTACK, {"Resource/SoldierDoll/sd_r_attack.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// ビッグドール
		{ enemy::red::BIG_WAKEUP, {"Resource/BigDoll/bd_r_wakeup.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_IDOL, {"Resource/BigDoll/bd_r_stand.png", 3, 1, 3, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_PATROL, {"Resource/BigDoll/bd_r_move.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_TACKLE, {"Resource/BigDoll/bd_r_tackle.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_HIPDROP, {"Resource/BigDoll/bd_r_hipdrop.png", 7, 1, 7, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},

		// 青い魂
		// ソルジャードール
		{ enemy::blue::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_b_wakeup.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_b_stand.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_b_move.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_ESCAPE, {"Resource/SoldierDoll/sd_b_escape.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// ビッグドール
		{ enemy::blue::BIG_WAKEUP, {"Resource/SoldierDoll/sd_b_wakeup.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_IDOL, {"Resource/SoldierDoll/sd_b_stand.png", 3, 1, 3, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_PATROL, {"Resource/SoldierDoll/sd_b_move.png", 4, 1, 4, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_ESCAPE, {"Resource/SoldierDoll/sd_b_escape.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
	};

	// 魂くんの各種モーション
	const graph::ResourceServer::DivGraphMap souls{
		{ soul::BLUE_SOUL, {"Resource/Soul/blue_soul.PNG.png", 1, 1, 1, soul::IMAGE_SIZE, soul::IMAGE_SIZE}},
		{ soul::B_FLOAT, {"Resource/Soul/b_soul.png", 5, 2, 5, soul::IMAGE_SIZE, soul::IMAGE_SIZE}},
		{ soul::RED_SOUL, {"Resource/Soul/red_soul.PNG.png", 1, 1, 1, soul::IMAGE_SIZE, soul::IMAGE_SIZE}},
		{ soul::R_FLOAT, {"Resource/Soul/r_soul.png", 5, 2, 5, soul::IMAGE_SIZE, soul::IMAGE_SIZE}},
	};

	// 背景等の一枚絵
	const graph::ResourceServer::DivGraphMap images{
		{ TITLE_LOGO, {"Resource/Title.png", 1, 1, 1, TITLE_IMAGE_W, TITLE_IMAGE_H}},

		{ image::BLACK, {"Resource/UI/black.png", 1, 1, 1, WINDOW_W, WINDOW_H}},

		// 各種UI
		{ TITLE_START1, {"Resource/UI/start.png", 1, 1, 1, TITLE_START_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_START2, {"Resource/UI/start1.png", 1, 1, 1, TITLE_START_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_EXIT1, {"Resource/UI/exit.png", 1, 1, 1, TITLE_EXIT_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_EXIT2, {"Resource/UI/exit1.png", 1, 1, 1, TITLE_EXIT_WIDTH, TITLE_UI_HEIGHT}},

		{ ui::CURSOR, {"Resource/UI/titleui.png", 5, 4, 20, ui::HP_SIZE, ui::HP_SIZE}},

		// HP
		{ ui::KEY_HP, {"Resource/UI/ui_hp.png", ui::HP_NUM, ui::HP_NUM, 25, ui::HP_SIZE, ui::HP_SIZE}},
		{ ui::KEY_RED, {"Resource/UI/ui_hp1.png", ui::HP_NUM, ui::HP_NUM, 25, ui::HP_SIZE, ui::HP_SIZE}},
		{ ui::KEY_BLUE, {"Resource/UI/ui_hp2.png", ui::HP_NUM, ui::HP_NUM, 25, ui::HP_SIZE, ui::HP_SIZE}},
		{ ui::KEY_BOX, {"Resource/UI/ui_hp_box.png", 1, 1, 1, 420, 200}},
	};

	const graph::ResourceServer::DivGraphMap gimmicks{
		{ gimmick::lever::KEY_LEVER, {"Resource/Gimmick/Lever/lever.png", 3, 2, 5, gimmick::lever::LEVER_SIZE, gimmick::lever::LEVER_SIZE}},
		{ gimmick::door::KEY_DOOR_LEVER, {"Resource/Gimmick/Lever/door.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE}},
	};

	// 各種エフェクト
	const graph::ResourceServer::DivGraphMap effects{
		{ effect::JUMP, {"Resource/effect/Player/Jump.png", 5, 1, 5, effect::JUMP_IMAGE, effect::JUMP_IMAGE}},	// ジャンプ

	};


	namespace titles {

		constexpr auto PATH_ALI = "ali/";
		constexpr auto PATH_ALO = "alo/";
		constexpr auto PATH_T = "t/";
		constexpr auto PATH_TLI = "tli/";
		constexpr auto PATH_TLO = "tlo/";

		constexpr auto FILE_ALI = "ali_";
		constexpr auto FILE_ALO = "alo_";
		constexpr auto FILE_T = "t_";
		constexpr auto FILE_TLI = "tli_";
		constexpr auto FILE_TLO = "tlo_";
	}

	void Loads::ResourceLoad() {
		graph::ResourceServer::LoadGraphList(div);
		graph::ResourceServer::LoadGraphList(enemys);
		graph::ResourceServer::LoadGraphList(souls);
		graph::ResourceServer::LoadGraphList(images);
		graph::ResourceServer::LoadGraphList(gimmicks);
		graph::ResourceServer::LoadGraphList(effects);

		std::string gpath = "Resource/effect/Logo/";
		graph::ResourceServer::SetLoadGraph(titles::TITLE_ALI, gpath + titles::PATH_ALI, titles::FILE_ALI, 34, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(titles::TITLE_ALO, gpath + titles::PATH_ALO, titles::FILE_ALO, 34, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(titles::TITLE_T, gpath + titles::PATH_T, titles::FILE_T, 30, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(titles::TITLE_TLI, gpath + titles::PATH_TLI, titles::FILE_TLI, 30, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(titles::TITLE_TLO, gpath + titles::PATH_TLO, titles::FILE_TLO, 30, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_1, background::STAGE1_PATH, background::STAGE1_FILE, 3, WINDOW_W, background::STAGE1_MAP_HEIGHT);
	}

	std::vector<ObjectValue> Loads::LoadScenario() {

		std::vector<ObjectValue> _stage1 {
			// プレイヤーの登録情報
			{ oscenario::OBJ_PLAYER, {static_cast<double>(stage_1::START_POS_X), static_cast<double>(stage_1::START_POS_Y)} },
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {500, 1950}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {2200, 1750}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {5000, 1800}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {6900, 1700}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {8000, 1800}, 0},

			// ギミック
			{ oscenario::OBJ_LEVER, {{8260, 1970}, {8525 , 1910}}, 0, gimmick::door::D_LEVER},
		};

		return _stage1;
	}
}
