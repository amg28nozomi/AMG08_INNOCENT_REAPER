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

namespace inr {

	// �ÓI�����o�ϐ���`
	int Game::_trgKey = 0;
	int Game::_frameCount = 0;

	constexpr auto PATH = "Resource/";
	// constexpr auto CHIP_FILE = "platformer_simpleA";
	constexpr auto CHIP_FILE = "stage1";

	
	// constexpr auto CHIP_FILE = "stage01";

	// �e��G�t�F�N�g
	const graph::ResourceServer::DivGraphMap effects{
		{ effect::JUMP, {"ResourceServer/effect/Player/Jump.png", 5, 1, 5, effect::JUMP_IMAGE, effect::JUMP_IMAGE}},	// �W�����v


		// AMG���S�A�j���[�V����

		// �`�[�����S�A�j���[�V����

		// �^�C�g���A�j���[�V����
	};

	const se::SoundServer::SoundMap player_se{
		{ {key::SOUND_PLAYER_HIT}, {"Resource/SE/Player/hit.wav", DX_PLAYTYPE_BACK}},
		{ {key::SOUND_PLAYER_DASH}, {"Resource/SE/Player/dash.wav", DX_PLAYTYPE_BACK}},
		// { {key::SOUND_PLAYER_RUN1}}
	};

	const se::SoundServer::SoundMap ses{
		{{key::SOUND_PLAYER_ROB}, {"Resource/SE/swing1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_RUN1}, {"Resource/SE/snapping1.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_GIVE}, {"Resource/SE/reflection.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_JUMP}, {"Resource/SE/putting_a_jar.mp3", DX_PLAYTYPE_BACK}},
		{{key::SOUND_PLAYER_FALL}, {"Resource/SE/defense1.mp3", DX_PLAYTYPE_BACK}},
	};

	// �M�~�b�NSE
	const se::SoundServer::SoundMap gimmick_se{
		{ {gimmick::lever::KEY_LEVER}, {"Resource/SE/Gimmick/lever.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::block::KEY_BLOCK}, {"Resource/SE/Gimmick/breakblock.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::crystal::KEY_CRYSTAL}, {"Resource/SE/Gimmick/crystal.wav", DX_PLAYTYPE_BACK}},
		{ {gimmick::door::KEY_DOOR}, {"Resource/SE/Gimmick/door.wav", DX_PLAYTYPE_BACK}},
	};

	Game::Game()
	{
		Init();
	}

	Game::~Game() {

	}

	// ������
	void Game::Init() {
		_joyKey = std::make_tuple(0, 0, 0);
		_trgKey = 0;
		_frameCount = 0;

		_objServer = std::make_unique<ObjectServer>();
		_scenario = std::make_unique<Scenario>(*this);
		_modeServer = std::make_unique<ModeServer>(*this);

		graph::ResourceServer::Init();
		Loads::ResourceLoad();
		// �ǂݍ���
		se::SoundServer::Init();
		se::SoundServer::LoadSoundMap(player_se);
		se::SoundServer::LoadSoundMap(ses);
		se::SoundServer::LoadSoundMap(gimmick_se);

		std::string filepath = PATH;
		std::string filename = CHIP_FILE;

		auto _mapC = std::make_unique<MapChips>(*this, filepath, filename);
		_mapChips = std::move(_mapC);
		// graph::ResourceServer::GetHandles("player_test", _graphs);
	}

	void Game::Input() {
		// �O�̓��͏����擾/�Q��
		auto beforeLR = std::get<LEVER_LR>(_joyKey);
		auto beforeUD = std::get<LEVER_UD>(_joyKey);
		auto beforeKey = std::get<KEY_JOYPAD>(_joyKey);

		// �A�i���O�X�e�B�b�N�̉��������擾
		GetJoypadAnalogInput(&std::get<LEVER_LR>(_joyKey), &std::get<LEVER_UD>(_joyKey), DX_INPUT_PAD1);
		// �L�[���͂��擾
		std::get<KEY_JOYPAD>(_joyKey) = GetJoypadInputState(DX_INPUT_PAD1);

		// �g���K���͂��擾
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
		// ��ʂɕ`�悳��Ȃ��B
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