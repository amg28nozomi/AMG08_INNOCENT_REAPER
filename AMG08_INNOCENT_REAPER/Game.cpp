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

namespace inr {

	// 静的メンバ変数定義
	int Game::_trgKey = 0;
	int Game::_frameCount = 0;

	constexpr auto PATH = "Resource/";
	 constexpr auto CHIP_FILE = "platformer_simpleA";
	//constexpr auto CHIP_FILE = "stage01";

	
	// constexpr auto CHIP_FILE = "stage01";

	// プレイヤーのモーション読み込み
	const graph::ResourceServer::DivGraphMap div{
		{PKEY_IDOL , {"Resource/Player/r_stand.png", 4, 4, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_RUN, {"Resource/Player/r_move.png", 4, 3, 9, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_DASH,{"Resourec/Player/r_dash.png", 4, 4, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_ROB , {"Resource/Player/r_rob.png", 4, 4, 13, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_GIVE, {"Resource/Player/r_give.png", 4, 4, 16, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_JUMP, {"Resource/Player/r_jump.png", 4, 2, 5, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_FALL, {"Resource/Player/r_fall.png", 4, 3, 10, PIMAGE_SIZE, PIMAGE_SIZE}},
		{PKEY_HIT, {"Resource/Player/r_hit.png", 4, 2, 7, PIMAGE_SIZE, PIMAGE_SIZE}},
		{BACK_GRAUND, {"Resource/背景ステージ１_ラフ1.png", 1, 1, 1, 1200, 1350 }},
	};

	const graph::ResourceServer::DivGraphMap soldierDoll{
		{ enemy::SOLDIER_PATROL, {"Resource/SoldierDoll/empty.png", 20, 1, 20, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::SOLDIER_PATROL, {"Resource/SoldierDoll/patrol.png", 6, 1, 6, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::SOLDIER_ESCAPE, {"Resource/SoldierDoll/attack.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
		{ enemy::SOLDIER_ATTACK, {"Resource/SoldierDoll/attack.png", 7, 1, 7, SOLDIER_IMAGE_W, SOLDIER_IMAGE_H}},
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

		// 読み込み
		graph::ResourceServer::Init();
		graph::ResourceServer::LoadGraphList(div);
		graph::ResourceServer::LoadGraphList(soldierDoll);
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