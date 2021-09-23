#pragma once
// #include <memory>
#include <tuple>
#include <memory>
#include "MapChips.h"

// #include <vector>

namespace inr {
	// �Q�[���I���t���O�p
	constexpr auto GAME_PLAYING = false;
	constexpr auto GAME_END = true;

	constexpr auto LEVER_LR = 0;
	constexpr auto LEVER_UD = 1;
	constexpr auto KEY_JOYPAD = 2;

	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;

	constexpr auto WINDOW_W = 1920;
	constexpr auto WINDOW_H = 1080;
	constexpr auto WINDOW_B = 32;

	constexpr auto HALF_WINDOW_W = 1920 / 2;
	constexpr auto HALF_WINDOW_H = 1080 / 2;
#ifdef _DEBUG
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	constexpr auto PLAYER_WIDTH = 50;
	constexpr auto PLAYER_HEIGHT = 110;

	namespace key {
		constexpr auto SOUND_NUM = "num";

		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";
		constexpr auto SOUND_PLAYER_ROB = "player_rob";
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";
		
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";

		constexpr auto SOUND_PLAYER_DASH = "player_dash";
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";	// ���̐؂�ւ�
		constexpr auto SOUND_PLAYER_HIT = "player_hit";
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";	// ���S��
	}

	namespace system {
		constexpr auto SOUDN_GAME_START = "game_start";
		constexpr auto SOUND_GAME_END = "game_end";
		constexpr auto SOUDN_CANCEL = "cancel"; // cancel
		constexpr auto SOUND_PAUSE = "pause";
		constexpr auto SOUND_SELECT = "select";	// �Z���N�g��

		constexpr auto MOVE_STAGE1 = "move_stage1";
		constexpr auto MOVE_STAGE2 = "move_stage2";
	}

	namespace bgm {
		constexpr auto SOUND_TITLE = "sound_title";
		constexpr auto SOUND_STAGE_0 = "sound_stage0";
		constexpr auto SOUND_STAGE_1 = "sound_stage1";
		constexpr auto SOUND_STAGE_2 = "sound_stage2";
		constexpr auto SOUND_STAGE_3 = "sound_stageb";
	}


	class Game {
	public:
		Game();
		~Game();
		void Init();	// ������
		void Input();	//�@����
		void Process();		// �X�V
		void Draw();	// �`��

		// �A�i���O�X�e�B�b�N�̉����͂��擾
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		// �A�i���O�X�e�B�b�N�̏c���͂��擾
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		// �A�i���O�X�e�B�b�N�̓��͏����擾�i�������j
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		// �L�[�{�[�h���͂̎擾�i�������j
		inline int GetInputKey() { return _inputKey; }
		// 
		inline int GetInputTrg() { return _inputTrg; }



		// �S���擾
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// �g���K�����擾
		inline int GetTrgKey() { return _trgKey; }
		// �t���[���J�E���^���擾
		inline int GetFrameCount() { return _frameCount; }
		// �v���O�������I�����邩�ۂ�
		inline bool GetEndFlag() { return _endFlag; }
		// �v���O�����I��������
		inline void ProgramEnd() { ++_endCount; }
		// Game�N���X�̎Q��
		inline Game& GetGame() { return *this; }

#ifdef _DEBUG
		inline bool IsDebugMode() { return _debug; }	// �����蔻���`�悷�邩�ۂ��H
#endif

		// �e��Q�ƃQ�b�^�[
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }

		// Vector2 RepairPosition(const Vector2 pos, const int width, const int height);

	private:
		// ObjectServer& _objServer;
		std::unique_ptr<ModeServer> _modeServer;	// ���[�h�T�[�o�[
		std::unique_ptr<ObjectServer> _objServer;	// �I�u�W�F�T�[�o�[
		std::unique_ptr<GimmickServer> _gServer;
		std::unique_ptr<MapChips> _mapChips;	// �}�b�v�`�b�v
		std::unique_ptr<Scenario> _scenario;	// �V�i���I

		Vector2 _worldPosition;	// world���W


		std::tuple<int, int, int> _joyKey;	// �W���C�p�b�h�̓��͏��
		int _inputKey;	// �L�[����
		int _inputTrg;	// �g���K����

		static int _trgKey;	// �g���K�[
		static int _frameCount;	// �t���[�����

		bool _endFlag;	// �v���O�������I�����邩�ۂ�
		int _endCount;	// �Q�[�����I������܂ł̃J�E���^

		void CountUp();
		
#ifdef _DEBUG
		bool _debug;	// �f�o�b�O��Ԃ��ۂ�
#endif
	};
}


