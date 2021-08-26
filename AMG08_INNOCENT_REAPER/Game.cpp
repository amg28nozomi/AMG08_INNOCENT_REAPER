#include "Game.h"
#include <memory>
#include <vector>
#include <tuple>
#include <DxLib.h>
#include "ObjectServer.h"
#include "ModeServer.h"
#include "Player.h"
#include "ResourceServer.h"
#include "SoundServer.h"
// #include "ModeServer.h"
#include "MapChips.h"

#include "EnemyBase.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Image.h"
#include "Scenario.h"
#include "EffectBase.h"

#include "Loads.h"

namespace inr {

	// 静的メンバ変数定義
	int Game::_trgKey = 0;
	int Game::_frameCount = 0;

	constexpr auto PATH = "Resource/";
	// constexpr auto CHIP_FILE = "platformer_simpleA";
	constexpr auto CHIP_FILE = "stage1";

	
	// constexpr auto CHIP_FILE = "stage01";

	// プレイヤーのモーション読み込み
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
	const graph::ResourceServer::DivGraphMap soldierDoll{
		// 魂が空の状態
		{ enemy::SOLDIER_EMPTY, {"Resource/SoldierDoll/sd_e_down.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// 赤い魂の状態
		{ enemy::red::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_r_wakeup.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_r_stand.png", 8, 1, 8, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_r_move.png", 9, 1, 9, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::red::SOLDIER_ATTACK, {"Resource/SoldierDoll/sd_r_attack.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		// 青い魂
		{ enemy::blue::SOLDIER_WAKEUP, {"Resource/SoldierDoll/sd_b_wakeup.png", 4, 1, 4, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_IDOL, {"Resource/SoldierDoll/sd_b_stand.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_PATROL, {"Resource/SoldierDoll/sd_b_move.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::blue::SOLDIER_ESCAPE, {"Resource/SoldierDoll/sd_b_escape.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
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
		{ BACK_GROUND, {"Resource/stage01ver2.png", 1, 1, 1, BACK_GROUND_W, BACK_GROUND_H}},

		{ AMG_LOGO, {"Resource/amg.png", 1, 1, 1, AMG_IMAGE, AMG_IMAGE}},	// AMGロゴ
		{ TEAM_LOGO, {"Resource/r_crystal.png", 1, 1, 1, TEAM_IMAGE, TEAM_IMAGE}},	// チームロゴ(画像差し替え)
		{ TITLE_BG, {"Resource/bg_title.png", 1, 1, 1, TITLE_IMAGE_WIDTH, TITLE_IMAGE_HEIGHT}},

		// 各種UI
		{ TITLE_START1, {"Resource/UI/start.png", 1, 1, 1, TITLE_START_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_START2, {"Resource/UI/start1.png", 1, 1, 1, TITLE_START_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_EXIT1, {"Resource/UI/exit.png", 1, 1, 1, TITLE_EXIT_WIDTH, TITLE_UI_HEIGHT}},
		{ TITLE_EXIT2, {"Resource/UI/exit1.png", 1, 1, 1, TITLE_EXIT_WIDTH, TITLE_UI_HEIGHT}},
	};

	// 各種エフェクト
	const graph::ResourceServer::DivGraphMap effects{
		{ effect::JUMP, {"ResourceServer/effect/Player/Jump.png", 5, 1, 5, effect::JUMP_IMAGE, effect::JUMP_IMAGE}},	// ジャンプ


		// AMGロゴアニメーション

		// チームロゴアニメーション

		// タイトルアニメーション
	};

	const se::SoundServer::SoundMap ses{
		{{key::SOUND_PLAYER_ROB}, {"Resource/SE/swing1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_RUN1}, {"Resource/SE/snapping1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_GIVE}, {"Resource/SE/reflection.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_JUMP}, {"Resource/SE/putting_a_jar.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_FALL}, {"Resource/SE/defense1.mp3", DX_PLAYTYPE_BACK}},
	};

	Game::Game()
	{
		Init();
	}

	Game::~Game() {

	}

	// 初期化
	void Game::Init() {
		_joyKey = std::make_tuple(0, 0, 0);
		_trgKey = 0;
		_frameCount = 0;

		_objServer = std::make_unique<ObjectServer>();
		_modeServer = std::make_unique<ModeServer>(*this);
		_scenario = std::make_unique<Scenario>(*this);

		graph::ResourceServer::Init();
		Loads::ResourceLoad();
		// 読み込み
		se::SoundServer::Init();
		se::SoundServer::LoadSoundMap(ses);

		std::string filepath = PATH;
		std::string filename = CHIP_FILE;

		auto _mapC = std::make_unique<MapChips>(*this, filepath, filename);
		_mapChips = std::move(_mapC);
		// graph::ResourceServer::GetHandles("player_test", _graphs);
	}

	void Game::Input() {
		// 前の入力情報を取得/参照
		auto beforeLR = std::get<LEVER_LR>(_joyKey);
		auto beforeUD = std::get<LEVER_UD>(_joyKey);
		auto beforeKey = std::get<KEY_JOYPAD>(_joyKey);

		// アナログスティックの押下情報を取得
		GetJoypadAnalogInput(&std::get<LEVER_LR>(_joyKey), &std::get<LEVER_UD>(_joyKey), DX_INPUT_PAD1);
		// キー入力を取得
		std::get<KEY_JOYPAD>(_joyKey) = GetJoypadInputState(DX_INPUT_PAD1);

		// トリガ入力を取得
		_trgKey = (std::get<KEY_JOYPAD>(_joyKey) ^ beforeKey) & std::get<KEY_JOYPAD>(_joyKey);
	}

	void Game::Process() {
		++_frameCount;
		_modeServer->Process();
		/*_mapChips->Process();
		_objServer->Process();*/
	}

	void Game::Draw() {
		
		// gra = _frameCount / 5 % static_cast<int>(_graphs.size());
		// auto motion = _graphs[gra];

		ClearDrawScreen();
		// 画面に描画されない。
		//DrawGraph(960, 540, motion, FALSE);
		// DrawGraph(960, 540, s[0], FALSE);
		// DrawRotaGraph(960, 540, 1.0, 0, motion, TRUE, FALSE);
		_modeServer->Draw();
		//_mapChips->Draw();
		//_objServer->Draw();
		ScreenFlip();
	}

	/*Vector2 Game::RepairPosition(const Vector2 pos, const int width, const int height) {
		return { 0, 0 };
	}*/
}