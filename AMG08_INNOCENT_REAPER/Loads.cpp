#include "Loads.h"
#include "ResourceServer.h"
#include "Game.h"
#include "Scenario.h"

#include "Player.h"
#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "BigDoll.h"
#include "CrowDoll.h"
#include "SoulSkin.h"
#include "EffectBase.h"
#include "Image.h"
#include "Particle_Image.h"
#include "Logo.h"
#include "FadeBlack.h"
#include "UI.h"
#include "SoulCursor.h"
#include "Lever.h"
#include "GimmickBase.h"
#include "ForeGround.h"
#include "Item.h"
#include "TutorialImage.h"

#include <vector>

namespace {
	constexpr auto NUM_SIMPLE = 1;

	// 生成座標
	namespace stage_1 {
		// constexpr auto START_POS_X = 200;
		constexpr auto START_POS_Y = 1970;
		constexpr auto START_POS_X = 8000;
		// constexpr auto START_POS_Y = 1700;
	}
}

namespace inr {

	const graph::ResourceServer::DivGraphMap div{
		{PKEY_IDOL , {"Resource/Player/r_stand.png", 7, 3, 15, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_RUN, {"Resource/Player/r_move.png", 7, 4, 25, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_DASH,{"Resource/Player/r_dash.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_ROB , {"Resource/Player/r_rob.png", 7, 2, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_GIVE, {"Resource/Player/r_give.png", 7, 3, 16, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_JUMP, {"Resource/Player/r_jump.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_FALL, {"Resource/Player/r_fall.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_HIT, {"Resource/Player/r_hit.png", 7, 1, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_CLIMB, {"Resource/Player/r_climb.png", 7, 2, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_DEATH, {"Resource/effect/Player/dead.png", 10, 4, 40, PLAYER_DEATH_IMAGE, PLAYER_DEATH_IMAGE}},
	};

	// ソルジャードールの各種モーション
	const graph::ResourceServer::DivGraphMap enemys {
		// 魂が空の状態
		{ enemy::SOLDIER_EMPTY, {"Resource/SoldierDoll/sd_e_down.png", 6, 4, 19, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::BIG_EMPTY, {"Resource/BigDoll/bd_e_down.png", 7, 1, 7, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		// 赤い魂の状態
		{ enemy::red::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_r_wakeup.png", 6, 3, 17, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_r_stand.png", 6, 2, 11, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_r_move.png", 6, 3, 13, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_ATTACK, {"Resource/SoldierDoll/sd_r_attack.png", 6, 2, 11, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// ビッグドール
		{ enemy::red::BIG_WAKEUP, {"Resource/BigDoll/bd_r_wakeup.png", 5, 1, enemy::red::big::WAKEUP_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_IDOL, {"Resource/BigDoll/bd_r_stand.png", 3, 1, enemy::red::big::IDOL_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_PATROL, {"Resource/BigDoll/bd_r_move.png", 5, 1, enemy::red::big::MOVE_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_TACKLE, {"Resource/BigDoll/bd_r_tackle.png", 5, 1, enemy::red::big::ATTACK_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::red::BIG_HIPDROP, {"Resource/BigDoll/bd_r_hipdrop.png", 7, 1, enemy::red::big::HIPDROP_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},

		// 青い魂
		// ソルジャードール
		{ enemy::blue::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_b_wakeup.png", 6, 3, 17, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_b_stand.png", 6, 2, 11, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_b_move.png", 6, 3, 13, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_ESCAPE, {"Resource/SoldierDoll/sd_b_escape.png", 6, 3, 13, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// ビッグドール
		{ enemy::blue::BIG_WAKEUP, {"Resource/BigDoll/bd_b_wakeup.png", 5, 1, enemy::blue::big::WAKEUP_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_IDOL, {"Resource/BigDoll/bd_b_stand.png", 3, 1, enemy::blue::big::IDOL_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_PATROL, {"Resource/BigDoll/bd_b_move.png", 4, 1, enemy::blue::big::PATROL_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_ESCAPE, {"Resource/BigDoll/bd_b_escape.png", 5, 1, enemy::blue::big::ESCAPE_SIZE, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},

		// クロウドール
		{ enemy::crowdoll::CROW_IDOL, {"Resource/CrowDoll/cd_float.png", 2, 7, enemy::crowdoll::motion::IDOL, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_RUSH, {"Resource/CrowDoll/cd_rush.png", 2, 18, enemy::crowdoll::motion::RUSH, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_BLINK, {"Resource/CrowDoll/cd_blink.png", 2, 7, enemy::crowdoll::motion::BLINK, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE}},
		{ enemy::crowdoll::CROW_DEBUFF, {"Resource/CrowDoll/cd_debuff.png", 2, 11, enemy::crowdoll::motion::DEBUF, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_GROWARM, {"Resource/CrowDoll/cd_growarm.png", 2, 5, enemy::crowdoll::motion::GROWARM, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_WINCE, {"Resource/CrowDoll/cd_wince.png", 2, 4, enemy::crowdoll::motion::WINCE, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_ROAR, {"Resource/CrowDoll/cd_roar.png", 2, 6, enemy::crowdoll::motion::ROAR, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE }},
		{ enemy::crowdoll::CROW_DOWN, {"Resource/CrowDoll/cd_dead.png", 2, 13, enemy::crowdoll::motion::DOWN, enemy::crowdoll::CROW_SIZE, enemy::crowdoll::CROW_SIZE}},
		{ enemy::crowdoll::CROW_ARM, {"Resource/CrowDoll/cd_arm.png", 2, 4, 7,  enemy::crowdoll::CROW_SIZE,  enemy::crowdoll::CROW_SIZE}},
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

		{ image::BLACK, {"Resource/UI/black.png", 1, 1, 1, 1922, 1360}},
		// ポーズ画面
		{ image::particle::BG_BLACK, {"Resource/UI/bg_back.png", 1, 1, 1, image::particle::BG_SIZE_W, image::particle::BG_SIZE_H}},
		{ image::particle::BG_OPERATION, {"Resource/UI/bg_operation.png", 1, 1, 1, image::particle::BG_SIZE_W, image::particle::BG_SIZE_H}},
		{ image::particle::CONTINUE, {"Resource/UI/continue.png", 1, 1, 1, image::particle::CONTINUE_W, image::particle::UI_H_1}},
		{ image::particle::CONTROLS, {"Resource/UI/controls.png", 1, 1, 1, image::particle::CONTROLS_W, image::particle::UI_H_1}},
		{ image::particle::QUIT_TO_TITLE, {"Resource/UI/quittotitle.png",1, 1, 1, image::particle::QUIT_TO_TITLE_W, image::particle::UI_H_2}},
		{ image::particle::CURSOR, {"Resource/UI/cursor.png", 4, 8, 30, image::particle::CURSOR_W, image::particle::CURSOR_H}},

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

		// 文章
		{ item::MESSAGE_0, {"Resource/Item/message1.png", 1, 1, 1, WINDOW_W, WINDOW_H}},
		{ item::MESSAGE_1, {"Resource/Item/message2.png", 1, 1, 1, WINDOW_W, WINDOW_H}},
		{ item::MESSAGE_2, {"Resource/Item/message3.png", 1, 1, 1, WINDOW_W, WINDOW_H}},
		{ item::MESSAGE_3, {"Resource/Item/message4.png", 1, 1, 1, WINDOW_W, WINDOW_H}},

		{ item::ITEM, {"Resource/Item/item.png", 13, 3, 28, item::ITEM_IMAGE, item::ITEM_IMAGE}},

		// 前景
		{ fgd::FORE_STAGE0, {"Resource/ForeGround/fg_stage0.png", 1, 1, 1, fgd::FORE0_W, fgd::FORE0_H}},
		{ fgd::FORE_STAGET, {"Resource/ForeGround/fg_stage0-2.png", 1, 1, 1, fgd::FORET_W, fgd::FORET_H} },
		{ fgd::FORE_STAGE1, {"Resource/ForeGround/fg_stage1.png", 1, 1, 1, fgd::FORE1_W, fgd::FORE1_H}},
		{ fgd::FORE_STAGE2, {"Resource/ForeGround/fg_stage2.png", 1, 1, 1, fgd::FORE2_W, fgd::FORE2_H}},
		{ fgd::FORE_STAGE2_1, {"Resource/ForeGround/fg_stage2-1.png", 1, 1, 1, fgd::FORE2_1_W, fgd::FORE2_1_H}},
		{ fgd::FORE_STAGE2_2,  {"Resource/ForeGround/fg_stage2-2.png", 1, 1, 1, fgd::FORE2_2_W, fgd::FORE2_2_H}},
		{ fgd::FORE_STAGE3,  {"Resource/ForeGround/fg_stageboss.png", 1, 1, 1, fgd::FORE2_1_W, fgd::FORE2_1_H}}, 

		{ fgd::FORE_OBJ, {"Resource/ForeGround/fg_stage0_f.png", 1, 1, 1, fgd::FORE_OBJ_W, fgd::FORE_OBJ_H}},

		// スタッフロール
		{ end::STAFF_ROLL, {"Resource/UI/stuffroll.png", 1, 1, 1, end::STAFF_ROLL_WIDTH, end::STAFF_ROLL_HEIGHT}},
		// 
		{ titles::TITLE_T, {"Resource/t.png", 6, 5, 30, WINDOW_W, WINDOW_H }},
		// チュートリアルUI
		{ tutorial::ROB, {"Resource/UI/Tutorial/ui_rob.png", 1, 1, 1, 374, 213 }},
		{ tutorial::GIVE, {"Resource/UI/Tutorial/ui_give.png", 1, 1, 1, 240, 163 }},
		{ tutorial::JUMP, {"Resource/UI/Tutorial/ui_jump.png", 1, 1, 1, 268, 143 }},
		{ tutorial::DASH, {"Resource/UI/Tutorial/ui_dash.png", 1, 1, 1, 271, 141 }},
		{ tutorial::GRAB, {"Resource/UI/Tutorial/ui_grab.png", 1, 1, 1, 219, 160 }},
		{ tutorial::INTERACTION, {"Resource/UI/Tutorial/ui_interaction", 1, 1, 1, 243, 143 }},
		{ tutorial::CRYSTAL, {"Resource/UI/Tutorial/rob_give.png", 1, 1, 1, 390, 161 }},
		{ tutorial::LEVER, {"Resource/UI/Tutorial/guide_font0.png", 1, 1, 1, 367, 140 }},
		{ tutorial::EMPTY, {"Resource/UI/Tutorial/guide_font1.png", 1, 1, 1, 342, 139 }},
	};

	const graph::ResourceServer::DivGraphMap gimmicks{
		{ gimmick::lever::KEY_LEVER, {"Resource/Gimmick/Lever/lever.png", 3, 2, 5, gimmick::lever::LEVER_SIZE, gimmick::lever::LEVER_SIZE}},
		{ gimmick::lever::KEY_LEVER_BOSS, {"Resource/Gimmick/Lever/bosslever.png", 5, 2, 10, gimmick::lever::BOSS_LEVER_SIZE,  gimmick::lever::BOSS_LEVER_SIZE}},
		{ gimmick::door::KEY_DOOR_LEVER, {"Resource/Gimmick/Lever/door.png", 3, 5, 14, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE}},
		{ gimmick::door::KEY_DOOR_BOSS, {"Resource/Gimmick/Lever/door_boss.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE}},
		{ gimmick::door::KEY_DOOR_RED, {"Resource/Gimmick/Crystal/r_door.png", 7, 3, 20, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE }},
		{ gimmick::door::KEY_DOOR_BLUE, {"Resource/Gimmick/Crystal/b_door.png", 7, 3, 20, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE }},
		{ gimmick::block::KEY_BLOCK, {"Resource/Gimmick/Break/breakblock3.png", 1, 1, 1, gimmick::block::BLOCK_SIZE_W, gimmick::block::BLOCK_SIZE_H}},
		{ gimmick::block::KEY_BREAK, {"Resource/Gimmick/Break/breakeffect.png", 4, 7, 26, gimmick::block::BRAKE_SIZE, gimmick::block::BRAKE_SIZE}},

		{ gimmick::crystal::type1::KEY_CRYSTAL_EMPTY, {"Resource/Gimmick/Crystal/e_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type1::KEY_CRYSTAL_RRD, {"Resource/Gimmick/Crystal/r_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type1::KEY_CRYSTAL_BLUE, {"Resource/Gimmick/Crystal/b_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRYSTAL_EMPTY, {"Resource/Gimmick/Crystal/e_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRYSTAL_RED, {"Resource/Gimmick/Crystal/r_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRTSTAL_BLUE, {"Resource/Gimmick/Crystal/b_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
	};

	// 各種エフェクト
	const graph::ResourceServer::DivGraphMap effects{
		{ effect::JUMP, {"Resource/effect/Player/Jump.png", 5, 1, 5, effect::JUMP_IMAGE_W, effect::JUMP_IMAGE_H}},	// ジャンプ
		{ effect::DASH, {"Resource/effect/Player/re_dash.png", 5, 1, 5, 360, 360}},
		{ effect::ROB, {"Resource/effect/Player/rob.png", 3, 3, 9, effect::ROB_IMAGE, effect::ROB_IMAGE}},	// 奪うアクション
		{ effect::HIT, {"Resource/effect/Player/hit.png", 5, 3, 15, effect::HIT_IMAGE_W, effect::HIT_IMAGE_H}},	// ダメージ
		{ effect::S_HIT, {"Resource/effect/Enemy/hit.png", 5, 2, 10, effect::SHIT_IMAGE, effect::SHIT_IMAGE}},
		{ effect::GIVE, {"Resource/effect/Player/give.png", 5, 4, 20, effect::GIVE_IMAGE, effect::GIVE_IMAGE }},

		{ effect::soldier::OMEN, {"Resource/effect/Enemy/sd_omen.png", 6, 3, effect::soldier::OMEN_NUMS, effect::soldier::OMEN_IMAGE, effect::soldier::OMEN_IMAGE}},
		{ effect::enemy::HITDROP, {"Resource/effect/Enemy/shockwave.png", 3, 8, effect::enemy::HIPDROP_MAX, effect::enemy::HIPDROP_WIDTH, effect::enemy::HIPDROP_HEIGHT}},

		{ effect::crow::ARM, {"Resource/effect/CrowDoll/arms.png", 5, 5, 24, effect::crow::ARM_WIDTH, effect::crow::ARM_HEIGHT}},
		{ effect::crow::RUSH, {"Resource/effect/CrowDoll/rush.png", 7, 7, 47, effect::crow::RUSH_WIDTH, effect::crow::RUSH_HEIGHT}},
		{ effect::crow::BLINK, {"Resource/effect/CrowDoll/blink.png", 5, 2, 10, effect::crow::BLINK_IMAGE, effect::crow::BLINK_IMAGE}},
		{ effect::crow::BLINK_ATTACK, {"Resource/effect/CrowDoll/blinkattack.png", 4, 4, effect::crow::BLINL_ATTACK_MAX, effect::crow::BLINK_ATTACK_W, effect::crow::BLINK_ATTACK_H}},
		{ effect::crow::AURA, {"Resource/effect/CrowDoll/bossaura.png", 8, 4, effect::crow::AURA_MAX, effect::crow::AURA_IMAGE, effect::crow::AURA_IMAGE}},
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
		//graph::ResourceServer::SetLoadGraph(titles::TITLE_ALI, gpath + titles::PATH_ALI, titles::FILE_ALI, 34, WINDOW_W, WINDOW_H);
		//graph::ResourceServer::SetLoadGraph(titles::TITLE_ALO, gpath + titles::PATH_ALO, titles::FILE_ALO, 34, WINDOW_W, WINDOW_H);
		//// graph::ResourceServer::SetLoadGraph(titles::TITLE_T, gpath + titles::PATH_T, titles::FILE_T, 30, WINDOW_W, WINDOW_H);
		//graph::ResourceServer::SetLoadGraph(titles::TITLE_TLI, gpath + titles::PATH_TLI, titles::FILE_TLI, 30, WINDOW_W, WINDOW_H);
		//graph::ResourceServer::SetLoadGraph(titles::TITLE_TLO, gpath + titles::PATH_TLO, titles::FILE_TLO, 30, WINDOW_W, WINDOW_H);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_S, background::STAGES_PATH, background::STAGES_FILE, 2, WINDOW_W, background::STAGES_HEIGHT);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_1, background::STAGE1_PATH, background::STAGE1_FILE, 3, WINDOW_W, background::STAGE1_MAP_HEIGHT);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_2, background::STAGE2_PATH, background::STAGE2_FILE, 4, WINDOW_W, background::STAGE2_MAP_HEIGHT);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_B, background::STAGEB_PATH, background::STAGEB_FILE, 4, WINDOW_W, WINDOW_H);
		std::string sui_path = "Resource/UI/";
		graph::ResourceServer::SetLoadGraph(image::particle::STAGE_UI, sui_path, image::particle::STAGE_UI, 3, image::particle::STAGE_UI_W, image::particle::STAGE_UI_H);
		std::string crow_path = "Resource/effect/CrowDoll/";
		graph::ResourceServer::SetLoadGraph(effect::crow::ROAR, crow_path + "roar/", "roar", 10, effect::crow::ROAR_WIDTH, effect::crow::ROAR_HEIGHT);
		std::string debuf_path = "Resource/effect/CrowDoll/debuf/";
		graph::ResourceServer::SetLoadGraph(effect::crow::DEBUF, debuf_path, "debuf_", effect::crow::DEBUF_MAX, WINDOW_W, WINDOW_H);
	}

	ImageMap Loads::LoadImages() {
		ImageMap imap{
			{ pvalue::ITEM_0, {item::MESSAGE_0, {HALF_WINDOW_W, HALF_WINDOW_H}, {false, true}} },
			{ pvalue::ITEM_1, {item::MESSAGE_1, {HALF_WINDOW_W, HALF_WINDOW_H}, {false, true}} },
			{ pvalue::ITEM_2, {item::MESSAGE_2, {HALF_WINDOW_W, HALF_WINDOW_H}, {false, true}} },
			{ pvalue::ITEM_3, {item::MESSAGE_3, {HALF_WINDOW_W, HALF_WINDOW_H}, {false, true}} },
		};
		return imap;
	}

	std::vector<TutorialValue> Loads::LoadTutorialF() {
		// ダッシュ
		// ジャンプ
		std::vector<TutorialValue> stageF = {
			{ {tutorial::DASH, {1450, 500}, {}}},
			{ {tutorial::JUMP, {1450, 400}, {}}},
		};
		return stageF;
	}

	std::vector<TutorialValue> Loads::LoadTutorialS() {
		// 
		std::vector<TutorialValue> stageS = {
			{{tutorial::GRAB, {350, 800}, {}}, 100, 200},
			{{tutorial::ROB, {1300, 500}, {}}, 300, 300},
			{{tutorial::GIVE, {2400, 500}, {}}, 300, 300},
			{{tutorial::EMPTY, {3050, 600}, {}}, 100, 200},
			{{tutorial::LEVER, {3300, 400}, {}}, 125, 150},
			{{tutorial::CRYSTAL, {4800, 550}, {}}, 300, 300},

		};
		return stageS;
	}

	std::vector<ImageValue> Loads::LoadForeGround1() {
		std::vector<ImageValue> fore_g = {
			{ fgd::FORE_STAGE0, { fgd::FORE0_W / 2, fgd::FORE0_H / 2 }, {}  },
			{ fgd::FORE_STAGET, { fgd::FORET_W / 2, fgd::FORET_H / 2}, {} },
			{ fgd::FORE_STAGE1, { fgd::FORE1_W / 2, fgd::FORE1_H / 2 }, {}  },
			{ fgd::FORE_STAGE2, { fgd::FORE2_W / 2, fgd::FORE2_H / 2 }, {}  },
			{ fgd::FORE_STAGE2_1, { fgd::FORE2_1_W / 2, fgd::FORE2_1_H / 2}, {}},
			{ fgd::FORE_STAGE2_2, { fgd::FORE2_2_W / 2, fgd::FORE2_2_H / 2}, {}},
			{ fgd::FORE_STAGE3, { fgd::FORE3_W / 2, fgd::FORE3_H / 2}, {}},
		};
		return fore_g;
	}

	// 封印の祭壇
	std::vector<ObjectValue> Loads::LoadScenarioS() {
		// ステージSの登場オブジェクトはソルジャードール1、ビッグドール1
		std::vector<ObjectValue> _stageS{
			{ oscenario::OBJ_SOLDIER_DOLL, {2000, 605}},
			{ oscenario::OBJ_BIG_DOLL, {2800, 905}},

			// ギミック（壊れる岩1, 水晶1）
			// 岩はyを-70する
			{ oscenario::OBJ_BLOCK, {3240, 740}, false, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_CRYSTAL, {{2620, 1890}, {2400, 1750}} , false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},

			// brooch
			{ oscenario::OBJ_ITEM, {640, 1915}, false, 0, {pvalue::ITEM_1, FALSE}},
		};
		return _stageS;
	}

	std::vector<ObjectValue> Loads::LoadScenarioS_1() {
		// 
		std::vector<ObjectValue> _stageT{
			{ oscenario::OBJ_SOLDIER_DOLL, {1500, 500}, true, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {2500, 500}, true },
			{ oscenario::OBJ_SOLDIER_DOLL, {3500, 300}, true, 1},

			{ oscenario::OBJ_LEVER, {{3450, 500}, {3900 , 420}}, false, 0, {gimmick::door::D_LEVER} },
			{ oscenario::OBJ_CRYSTAL, { {4550, 730}, {4320, 670}}, false, 1, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},
			{ oscenario::OBJ_CRYSTAL, { {5000, 730}, {5220, 670}}, false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},
		};
		return _stageT;
	}

	// 荒廃し街
	std::vector<ObjectValue> Loads::LoadScenario1() {

		std::vector<ObjectValue> _stage1 {
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {500, 1950}, true, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {2200, 1750}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {5000, 1800}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {6900, 1700}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {8000, 1800}, false, 0},

			// ギミック
			{ oscenario::OBJ_LEVER, {{8260, 1970}, {8525 , 1910}}, false, 0, {gimmick::door::D_LEVER}},

			{ oscenario::OBJ_ITEM, {5640, 325}, false, 0, {pvalue::ITEM_3, FALSE}},
		};

		return _stage1;
	}

	// 森林-1
	std::vector<ObjectValue> Loads::LoadScenario2() {
		// ソルジャードール9(赤5、青3、空1)
		// ビッグドール3(赤2、青1)
		// レバー/扉(2)
		// 岩(1-3)
		// 水晶(2/4)
		std::vector<ObjectValue> _stage{
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {3520, 780}, false, 0},
			{ oscenario::OBJ_SOLDIER_DOLL, {1400, 1500}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {7640, 1970}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {1640, 1500}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {2120, 860}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {7320, 1970}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {7960, 340}, false, 2},

			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, {1640, 840}, false, 1},
			{ oscenario::OBJ_BIG_DOLL, {4000, 780}, false, 1},
			{ oscenario::OBJ_BIG_DOLL, {6840, 1970}, false, 1},
			// ギミック
			{ oscenario::OBJ_LEVER, {{2200, 1970}, {2500, 1900}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{3680, 770}, {3200, 700}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			// ボス扉
			{ oscenario::OBJ_DOOR, {8470, 1190}, false, 0, {gimmick::door::D_BOSS}},
		};
		return _stage;
	}

	// 森林-2
	std::vector<ObjectValue> Loads::LoadScenario2_1() {
		std::vector<ObjectValue> _stage{
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, { 1540, 1870}, enemy::MOVE_LEFT, 0 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 150, 200}, enemy::MOVE_LEFT, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 2060, 900}, false, 1 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 1660, 1390}, false, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 3760, 190}, false },
			{ oscenario::OBJ_SOLDIER_DOLL, { 6220, 1910}, false, 1 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 6620, 1340}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, { 7940, 630}, false, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 8820, 270}, false, 1 },
			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, { 1130, 560}, false, 1 },
			{ oscenario::OBJ_BIG_DOLL, { 4560, 820}, false, 2 },
			{ oscenario::OBJ_BIG_DOLL, { 5240, 820}, false, 1 },
			// レバー
			{ oscenario::OBJ_LEVER, {{6880, 1370}, {6840, 830}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{8320, 1210}, {9000, 1150}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			// 壊れる岩
			{ oscenario::OBJ_BLOCK, {1440, 510}, false,  0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			// 水晶（x-90）
			{ oscenario::OBJ_CRYSTAL, {{2870, 1570}, {3180, 1410}, {3180, 1810}} , false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{8370, 610}, {9000, 230}, {9000, 590}} , false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},

			{ oscenario::OBJ_ITEM, {3840, 215}, false, 0, {pvalue::ITEM_0, FALSE}},
			{ oscenario::OBJ_ITEM, {8280, 1965}, false, 0, {pvalue::ITEM_2, FALSE}},
		};
		return _stage;
	}
	// 森林-3
	std::vector<ObjectValue> Loads::LoadScenario2_2() {
		std::vector<ObjectValue> _stage{
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {2130, 310}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1430, 910}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {670, 1590}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1230, 1590}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {2830, 1470}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {3350, 2030}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {830, 2390}, false, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1310, 2390}, false, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {3110, 2950}, false, 2},
			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, {2720, 910}, false, 1},
			{ oscenario::OBJ_BIG_DOLL, {2240, 2030}, false, 2},
			{ oscenario::OBJ_BIG_DOLL, {490, 2390}, false, 1},
			{ oscenario::OBJ_BIG_DOLL, {1320, 2990}},
			{ oscenario::OBJ_BIG_DOLL, {3320, 2950}, false, 1},
			// レバー（y -90）
			{ oscenario::OBJ_LEVER, {{3650 ,2970}, {630, 870}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{150 ,3010}, {3190, 870}}, false, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			// ボスレバーのみ
			{ oscenario::OBJ_LEVER, {3440, 930}, false, 0, {gimmick::door::D_BOSS}},
			// 壊れる岩
			{ oscenario::OBJ_BLOCK, {2650, 2050}, false, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_BLOCK, {920, 3010}, false, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_BLOCK, {1720, 3010}, false, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			// 水晶
			{ oscenario::OBJ_CRYSTAL, {{910, 1610}, {350, 1550}, {1430, 1550}} , false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{910, 2410}, {1430, 2350}, {270, 2350}} , false, 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{630, 3010}, {310, 2950}} , false, 0, {gimmick::door::D_BLUE, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},
		};
		return _stage;
	}
	// ボスステージ
	std::vector<ObjectValue> Loads::LoadScenarioB() {
		std::vector<ObjectValue> _stage{
			{ oscenario::OBJ_SOLDIER_DOLL, {1210, 870}},
			{ oscenario::OBJ_SOLDIER_DOLL, {2880, 870}},

			{ oscenario::OBJ_BIG_DOLL, {1770, 870}},
			{ oscenario::OBJ_BIG_DOLL, {3090, 870}},
			{ oscenario::OBJ_BIG_DOLL, {3290, 870}},

			{ oscenario::OBJ_CROW_DOLL, {5450, 840}},
		};
		return _stage;
	}

}
