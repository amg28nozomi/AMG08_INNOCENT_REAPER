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

#include "GimmickBase.h"

#include "Loads.h"

namespace {
	constexpr auto END_MAX = 90;
}

namespace inr {

	// 静的メンバ変数定義
	int Game::_trgKey = 0;
	int Game::_frameCount = 0;

	constexpr auto PATH = "Resource/";
	// constexpr auto CHIP_FILE = "platformer_simpleA";
	constexpr auto CHIP_FILE_S = "stage0";
	constexpr auto CHIP_FILE_1 = "stage1";
	constexpr auto CHIP_FILE_2_1 = "stage2";
	constexpr auto CHIP_FILE_2_2 = "stage2-1";
	constexpr auto CHIP_FILE_2_3 = "stage2-2";

	
	// constexpr auto CHIP_FILE = "stage01";

	// 各種エフェクト
	const graph::ResourceServer::DivGraphMap effects{
		{ effect::JUMP, {"ResourceServer/effect/Player/Jump.png", 5, 1, 5, effect::JUMP_IMAGE_W, effect::JUMP_IMAGE_H}},	// ジャンプ


		// AMGロゴアニメーション

		// チームロゴアニメーション

		// タイトルアニメーション
	};

	const se::SoundServer::SoundMap player_se{
		{ {key::SOUND_PLAYER_HIT}, {"Resource/SE/Player/damege.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_DASH}, {"Resource/SE/Player/dash.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_DEAD}, {"Resource/SE/Player/dead_01.wav", DX_PLAYTYPE_BACK}},
		// { {key::SOUND_PLAYER_RUN1}}
	};

	const se::SoundServer::SoundMap ses{
		{{key::SOUND_PLAYER_ROB}, {"Resource/SE/swing1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_RUN1}, {"Resource/SE/snapping1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_GIVE}, {"Resource/SE/reflection.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_JUMP}, {"Resource/SE/putting_a_jar.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_FALL}, {"Resource/SE/defense1.mp3", DX_PLAYTYPE_BACK}},
	};

	// ギミックSE
	const se::SoundServer::SoundMap gimmick_se{
		{ {gimmick::lever::KEY_LEVER}, {"Resource/SE/Gimmick/lever.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::block::KEY_BLOCK}, {"Resource/SE/Gimmick/breakblok.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::crystal::KEY_CRYSTAL}, {"Resource/SE/Gimmick/crystal.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::door::KEY_DOOR}, {"Resource/SE/Gimmick/door.wav", DX_PLAYTYPE_BACK}},

		{ {gimmick::door::SE_CLOSE_DOOR}, {"Resource/SE/Gimmick/door_close.wav", DX_PLAYTYPE_BACK}},
	};

	// System音
	const se::SoundServer::SoundMap system_se{
		{ {system::SOUDN_GAME_START}, {"Resource/SE/System/gamestart.wav", DX_PLAYTYPE_BACK}},
		{ {system::SOUND_GAME_END}, {"Resource/SE/System/1.wav", DX_PLAYTYPE_BACK}},
		{ {system::SOUDN_CANCEL}, {"Resource/SE/System/cancel.wav", DX_PLAYTYPE_BACK}},
		{ {system::SOUND_PAUSE}, {"Resource/SE/System/pose.wav", DX_PLAYTYPE_BACK}},
	};

	const se::SoundServer::SoundMap bgms{
		{ {bgm::SOUND_TITLE}, {"Resource/SE/BGM/title.wav", DX_PLAYTYPE_LOOP}},	// タイトルBGM
		{ {bgm::SOUND_STAGE_0}, {"Resource/SE/BGM/stage0.wav", DX_PLAYTYPE_LOOP}},	// ステージ0
		{ {bgm::SOUND_STAGE_1}, {"Resource/SE/BGM/stage1.wav", DX_PLAYTYPE_LOOP}},	// ステージ1
		{ {bgm::SOUND_STAGE_2}, {"Resource/SE/BGM/stage2.wav", DX_PLAYTYPE_LOOP}},	// ステージ2
		{ {bgm::SOUND_STAGE_3}, {"Resource/SE/BGM/stageboss_batle.wav", DX_PLAYTYPE_LOOP}},
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

		graph::ResourceServer::Init();
		Loads::ResourceLoad();
		// 読み込み
		se::SoundServer::Init();
		se::SoundServer::LoadSoundMap(player_se);
		se::SoundServer::LoadSoundMap(ses);
		se::SoundServer::LoadSoundMap(gimmick_se);
		se::SoundServer::LoadSoundMap(system_se);
		se::SoundServer::LoadSoundMap(bgms);

		_objServer = std::make_unique<ObjectServer>();
		_scenario = std::make_unique<Scenario>(*this);
		_modeServer = std::make_unique<ModeServer>(*this);

		/*std::string filepath = PATH;
		std::string filename = CHIP_FILE_S;*/
		//std::string filename = CHIP_FILE_2_1;

		_mapChips = std::make_unique<MapChips>(*this);
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
		CountUp();
		_modeServer->Process();
		/*_mapChips->Process();
		_objServer->Process();*/
	}

	void Game::Draw() {
		ClearDrawScreen();
		_modeServer->Draw();
		ScreenFlip();
	}

	void Game::CountUp() {
		++_frameCount;
		if (_endCount == 0) return;
		++_endCount;
		if (_endCount == END_MAX) {
			_endFlag = true;
		}
	}
}