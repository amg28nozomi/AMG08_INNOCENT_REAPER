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
#include "CrowDoll.h"

#include "GimmickBase.h"
#include "GimmickServer.h"
#include "Pause_UI.h"
#include "Loads.h"

namespace {
	constexpr auto END_MAX = 90;

	constexpr auto DEBUG_OFF = false;
	constexpr auto DEBUG_PN = true;
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
		{ {key::SOUND_PLAYER_GIVE_TRUE}, {"Resource/SE/Player/give_true.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_GIVE_FALSE}, {"Resource/SE/Player/give_false.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_SWITCH}, {"Resource/SE/Player/switchsoul.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_JUMP}, {"Resource/SE/Player/jump.wav", DX_PLAYTYPE_BACK}},
		// { {key::SOUND_PLAYER_RUN1}}
	};

	const se::SoundServer::SoundMap enemy_se{
		{ {enemy::soldier::ATTACK_SE}, {"Resource/SE/SoldierDoll/sd_attack.wav", DX_PLAYTYPE_BACK}},
		{ {enemy::soldier::ATTACK_VOICE}, {"Resource/SE/SoldierDoll/sd_attackvoice.wav", DX_PLAYTYPE_BACK}},
		{ {enemy::soldier::DOWN}, {"Resource/SE/SoldierDoll/sd_down.wav", DX_PLAYTYPE_BACK}},
		{ {enemy::soldier::ESCAPE_SE}, {"Resource/SE/SoldierDoll/sd_escape.wav", DX_PLAYTYPE_BACK}},
		{ {enemy::soldier::ESCAPE_VOICE}, {"Resource/SE/SoldierDoll/sd_escapevoice.wav", DX_PLAYTYPE_BACK}},
		{ {enemy::soldier::MOVE}, {"Resource/SE/SoldierDoll/sd_move.wav", DX_PLAYTYPE_BACK}},
	};

	const se::SoundServer::SoundMap ses {
		{{key::SOUND_PLAYER_ROB}, {"Resource/SE/swing1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_RUN1}, {"Resource/SE/Player/move_stage1.wav", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_RUN2}, {"Resource/SE/Player/move_stage2.wav", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_FALL1}, {"Resource/SE/Player/landing_stage1.wav", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_FALL2}, {"Resource/SE/Player/landing_stage2.wav", DX_PLAYTYPE_BACK}},

		{{enemy::crowdoll::SE_VOICE}, {"Resource/SE/CrowDoll/cd_voice.wav", DX_PLAYTYPE_BACK}},
		{{enemy::crowdoll::SE_RASH}, {"Resource/SE/CrowDoll/cd_rush.wav", DX_PLAYTYPE_BACK} },
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
		{ {system::CURSOR_MOVE}, {"Resource/SE/System/cursormove.wav", DX_PLAYTYPE_BACK}},
		{ {system::SOUND_SELECT}, {"Resource/SE/System/select_0.wav", DX_PLAYTYPE_BACK}},

		{ {system::MOVE_STAGE1}, {"Resource/SE/System/change_stage1.wav", DX_PLAYTYPE_BACK}},
		{ {system::MOVE_STAGE2}, {"Resource/SE/System/change_stage2.wav", DX_PLAYTYPE_BACK}},
	};

	const se::SoundServer::SoundMap bgms{
		{ {bgm::SOUND_TITLE}, {"Resource/SE/BGM/title.wav", DX_PLAYTYPE_LOOP}},	// タイトルBGM
		{ {bgm::SOUND_STAGE_0}, {"Resource/SE/BGM/stage0.wav", DX_PLAYTYPE_LOOP}},	// ステージ0
		{ {bgm::SOUND_STAGE_1}, {"Resource/SE/BGM/stage1.wav", DX_PLAYTYPE_LOOP}},	// ステージ1
		{ {bgm::SOUND_STAGE_2}, {"Resource/SE/BGM/stage2.wav", DX_PLAYTYPE_LOOP}},	// ステージ2
		{ {bgm::SOUND_STAGE_3}, {"Resource/SE/BGM/stageboss_batle.wav", DX_PLAYTYPE_LOOP}},
	};

	Game::Game() {
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
		se::SoundServer::LoadSoundMap(enemy_se);
		se::SoundServer::LoadSoundMap(ses);
		se::SoundServer::LoadSoundMap(gimmick_se);
		se::SoundServer::LoadSoundMap(system_se);
		se::SoundServer::LoadSoundMap(bgms);

		_objServer = std::make_unique<ObjectServer>();
		_scenario = std::make_unique<Scenario>(*this);
		_modeServer = std::make_unique<ModeServer>(*this);
		_gServer = std::make_unique<GimmickServer>(*this);

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

#ifdef _DEBUG
		// 入力があった場合、デバッグモードを切り替える
		if (_trgKey == PAD_INPUT_7) _debug = !_debug;
#endif

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