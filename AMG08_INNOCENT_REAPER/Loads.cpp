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
#include "Particle_Image.h"
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
	};

	// ソルジャードールの各種モーション
	const graph::ResourceServer::DivGraphMap enemys {
		// 魂が空の状態
		{ enemy::SOLDIER_EMPTY, {"Resource/SoldierDoll/sd_e_down.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::BIG_EMPTY, {"Resource/BigDoll/bd_e_down.png", 7, 1, 7, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		// 赤い魂の状態
		{ enemy::red::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_r_wakeup.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_r_stand.png", 6, 2, 11, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
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
		{ enemy::blue::BIG_WAKEUP, {"Resource/BigDoll/bd_b_wakeup.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_IDOL, {"Resource/BigDoll/bd_b_stand.png", 3, 1, 3, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_PATROL, {"Resource/BigDoll/bd_b_move.png", 4, 1, 4, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
		{ enemy::blue::BIG_ESCAPE, {"Resource/BigDoll/bd_b_escape.png", 5, 1, 5, enemy::BIG_IMAGE, enemy::BIG_IMAGE}},
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
		{ image::particle::BG_BLACK, {"Resource/UI/bg_black.png", 1, 1, 1, image::particle::BG_SIZE_W, image::particle::BG_SIZE_H}},
		{ image::particle::BG_OPERATION, {"Resource/UI/operation.png", 1, 1, 1, image::particle::BG_SIZE_W, image::particle::BG_SIZE_H}},
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
	};

	const graph::ResourceServer::DivGraphMap gimmicks{
		{ gimmick::lever::KEY_LEVER, {"Resource/Gimmick/Lever/lever.png", 3, 2, 5, gimmick::lever::LEVER_SIZE, gimmick::lever::LEVER_SIZE}},
		{ gimmick::lever::KEY_LEVER_BOSS, {"Resource/Gimmick/Lever/lever_boss.png", 3, 2, 5, gimmick::lever::LEVER_SIZE,  gimmick::lever::LEVER_SIZE}},
		{ gimmick::door::KEY_DOOR_LEVER, {"Resource/Gimmick/Lever/door.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE}},
		{ gimmick::door::KEY_DOOR_BOSS, {"Resource/Gimmcik/Lever/door_boss.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE}},
		{ gimmick::door::KEY_DOOR_RED, {"Resource/Gimmick/Crystal/r_door.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE }},
		{ gimmick::door::KEY_DOOR_BLUE, {"Resource/Gimmick/Crystal/b_door.png", 1, 1, 1, gimmick::door::DOOR_SIZE, gimmick::door::DOOR_SIZE }},
		{ gimmick::block::KEY_BLOCK, {"Resource/Gimmick/Break/breakblock.png", 1, 1, 1, gimmick::block::BLOCK_SIZE, gimmick::block::BLOCK_SIZE}},

		{ gimmick::crystal::type1::KEY_CRYSTAL_EMPTY, {"Resource/Gimmick/Crystal/e_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type1::KEY_CRYSTAL_RRD, {"Resource/Gimmick/Crystal/r_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type1::KEY_CRYSTAL_BLUE, {"Resource/Gimmick/Crystal/b_crystal.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRYSTAL_EMPTY, {"Resource/Gimmick/Crystal/e_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRYSTAL_RED, {"Resource/Gimmick/Crystal/r_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
		{ gimmick::crystal::type2::KEY_CRTSTAL_BLUE, {"Resource/Gimmick/Crystal/b_crystal_v2.png", 1, 1, 1, gimmick::crystal::CRYSTAL_SIZE, gimmick::crystal::CRYSTAL_SIZE}},
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
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_S, background::STAGES_PATH, background::STAGES_FILE, 2, WINDOW_W, background::STAGES_HEIGHT);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_1, background::STAGE1_PATH, background::STAGE1_FILE, 3, WINDOW_W, background::STAGE1_MAP_HEIGHT);
		graph::ResourceServer::SetLoadGraph(background::BACK_GROUND_2, background::STAGE2_PATH, background::STAGE2_FILE, 4, WINDOW_W, background::STAGE2_MAP_HEIGHT);
	}

	// 封印の祭壇
	std::vector<ObjectValue> Loads::LoadScenarioS() {
		// ステージSの登場オブジェクトはソルジャードール1、ビッグドール1
		std::vector<ObjectValue> _stageS{
			{ oscenario::OBJ_SOLDIER_DOLL, {2000, 605}},
			{ oscenario::OBJ_BIG_DOLL, {2800, 905}},

			// ギミック（壊れる岩1, 水晶1）
			{ oscenario::OBJ_BLOCK, {3180, 890}, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_CRYSTAL, {{2620, 1890}, {2400, 1750}} , 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},
		};
		return _stageS;
		/*std::vector<ObjectValue>_stageS {
			{ oscenario::OBJ_SOLDIER_DOLL, {}}
		}

		return _stageS;*/
	}

	// 荒廃し街
	std::vector<ObjectValue> Loads::LoadScenario1() {

		std::vector<ObjectValue> _stage1 {
			// プレイヤーの登録情報
			// { oscenario::OBJ_PLAYER, { 200,800}, 0 },
			// { oscenario::OBJ_PLAYER, {static_cast<double>(stage_1::START_POS_X), static_cast<double>(stage_1::START_POS_Y)}, 0 },
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {500, 1950}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {2200, 1750}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {5000, 1800}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {6900, 1700}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {8000, 1800}, 0},

			// ギミック
			{ oscenario::OBJ_LEVER, {{8260, 1970}, {8525 , 1910}}, 0, {gimmick::door::D_LEVER}},
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
			{ oscenario::OBJ_SOLDIER_DOLL, {3520, 780}, 0},
			{ oscenario::OBJ_SOLDIER_DOLL, {1400, 1500}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {7640, 1970}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {1640, 1500}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {2120, 860}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {7320, 1970}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {7960, 340}, 2},

			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, {1640, 840}, 1},
			{ oscenario::OBJ_BIG_DOLL, {4000, 780}, 1},
			{ oscenario::OBJ_BIG_DOLL, {6840, 1970}, 1},
			// ギミック
			{ oscenario::OBJ_LEVER, {{2200, 1970}, {2500, 1900}},  0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{3680, 770}, {3200, 700}},  0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
		};
		return _stage;
	}

	// 森林-2
	std::vector<ObjectValue> Loads::LoadScenario2_1() {
		std::vector<ObjectValue> _stage{
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, { 1540, 1870}, 1 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 1260, 150}, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 2060, 900}, 1 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 1660, 1390}, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 3760, 190}},
			{ oscenario::OBJ_SOLDIER_DOLL, { 6220, 1910}, 1 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 6620, 1340}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, { 7940, 630}, 2 },
			{ oscenario::OBJ_SOLDIER_DOLL, { 8820, 270}, 1 },
			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, { 1130, 560}, 1 },
			{ oscenario::OBJ_BIG_DOLL, { 4560, 820}, 2 },
			{ oscenario::OBJ_BIG_DOLL, { 5240, 820}, 1 },
			// レバー
			{ oscenario::OBJ_LEVER, {{6880, 1370}, {6840, 830}},  0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{8320, 1210}, {9000, 1150}},  0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			// 壊れる岩
			{ oscenario::OBJ_BLOCK, {1440, 530}, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			// 水晶（x-90）
			{ oscenario::OBJ_CRYSTAL, {{2870, 1570}, {3180, 1410}, {3180, 1810}} , 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{8370, 610}, {9000, 230}, {9000, 590}} , 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
		};
		return _stage;
	}
	// 森林-3
	std::vector<ObjectValue> Loads::LoadScenario2_2() {
		std::vector<ObjectValue> _stage{
			// ソルジャードール
			{ oscenario::OBJ_SOLDIER_DOLL, {2130, 310}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1430, 910}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {670, 1590}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1230, 1590}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {2830, 1470}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {3350, 2030}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {830, 2390}, 2},
			{ oscenario::OBJ_SOLDIER_DOLL, {1310, 2390}, 1},
			{ oscenario::OBJ_SOLDIER_DOLL, {3110, 2950}, 2},
			// ビッグドール
			{ oscenario::OBJ_BIG_DOLL, {2720, 910}, 1},
			{ oscenario::OBJ_BIG_DOLL, {2240, 2030}, 2},
			{ oscenario::OBJ_BIG_DOLL, {490, 2390}, 1},
			{ oscenario::OBJ_BIG_DOLL, {1320, 2990}},
			{ oscenario::OBJ_BIG_DOLL, {3320, 2950}, 1},
			// レバー（y -90）
			{ oscenario::OBJ_LEVER, {{3650 ,2970}, {630, 870}}, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_LEVER, {{150 ,3010}, {3190, 870}}, 0, {gimmick::door::D_LEVER, oscenario::gimmick::FLAG_FALSE}},
			// 壊れる岩
			{ oscenario::OBJ_BLOCK, {2670, 2050}, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_BLOCK, {990, 3010}, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			{ oscenario::OBJ_BLOCK, {1790, 3010}, 0, {oscenario::gimmick::TYPE_BLOCK, oscenario::gimmick::FLAG_FALSE}},
			// 水晶
			{ oscenario::OBJ_CRYSTAL, {{910, 1610}, {350, 1550}, {1430, 1550}} , 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{910, 2410}, {1430, 2350}, {270, 2350}} , 0, {gimmick::door::D_RED, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_2, {oscenario::gimmick::crystal::DOOR_RED, oscenario::gimmick::crystal::DOOR_BLUE}}},
			{ oscenario::OBJ_CRYSTAL, {{630, 3010}, {310, 2950}} , 0, {gimmick::door::D_BLUE, oscenario::gimmick::FLAG_FALSE, oscenario::gimmick::crystal::TYPE_STAGE_0, {oscenario::gimmick::crystal::DOOR_RED}}},
		};
		return _stage;
	}
	// ボスステージ
	std::vector<ObjectValue> Loads::LoadScenarioB() {
		std::vector<ObjectValue> _stage;
		return _stage;
	}

}
