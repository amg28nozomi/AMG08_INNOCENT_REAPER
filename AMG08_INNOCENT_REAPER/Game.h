/*****************************************************************//**
 * \file   Game.h
 * \brief  �Q�[���N���X
 *		   �{�Q�[���̏������񂵂Ă��܂�
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <memory>
#include "MapChips.h"

namespace inr {
	// �Q�[���I���t���O�p
	constexpr auto GAME_PLAYING = false;
	constexpr auto GAME_END = true;
	// �W���C�p�b�h�̓��͏����o���p�ԍ�
	constexpr auto LEVER_LR = 0;		// �c����
	constexpr auto LEVER_UD = 1;		// ������
	constexpr auto KEY_JOYPAD = 2;		// �{�^������
	// ��d�C���N���[�h�h�~
	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;
	// ��ʏ��
	constexpr auto WINDOW_W = 1920;				// ���T�C�Y
	constexpr auto WINDOW_H = 1080;				// �c�T�C�Y
	constexpr auto WINDOW_B = 32;				// �r�b�g��
	// �n�[�t�T�C�Y
	constexpr auto HALF_WINDOW_W = 1920 / 2;
	constexpr auto HALF_WINDOW_H = 1080 / 2;
#ifdef _DEBUG
	// �f�o�b�O���̉摜�T�C�Y
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	// ���@�̃T�C�Y���
	constexpr auto PLAYER_WIDTH = 50;
	constexpr auto PLAYER_HEIGHT = 110;

	namespace key {
		// �����p�L�[(SoundServer)
		constexpr auto SOUND_NUM = "num";								// ��������
		// ���@SE
		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";				// �ړ���1
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";				// �ړ���2
		constexpr auto SOUND_PLAYER_ROB = "player_rob";					// �D��
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";		// �^����(����)
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";	// �^����(���s)
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";				// �W�����v
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";			// ���n1
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";			// ���n2
		constexpr auto SOUND_PLAYER_DASH = "player_dash";				// �_�b�V��
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";			// ���̐؂�ւ�
		constexpr auto SOUND_PLAYER_HIT = "player_hit";					// �_���[�W
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";				// ���S
	}
	namespace system {
		// �V�X�e��SE
		constexpr auto SOUDN_GAME_START = "game_start";					// �Q�[���J�n
		constexpr auto SOUND_GAME_END = "game_end";						// �Q�[���I��
		constexpr auto SOUDN_CANCEL = "cancel";							// �L�����Z��
		constexpr auto SOUND_PAUSE = "pause";							// �|�[�Y
		constexpr auto SOUND_SELECT = "select";							// �Z���N�g��
		constexpr auto MOVE_STAGE1 = "move_stage1";						// �X�e�[�W�J��1
		constexpr auto MOVE_STAGE2 = "move_stage2";						// �X�e�[�W�J��2
	}
	namespace bgm {
		// BGM
		constexpr auto SOUND_TITLE = "sound_title";						// �^�C�g��
		constexpr auto SOUND_STAGE_0 = "sound_stage0";					// ����̍Ւd
		constexpr auto SOUND_STAGE_1 = "sound_stage1";					// �r�p������
		constexpr auto SOUND_STAGE_2 = "sound_stage2";					// �_��̐X
		constexpr auto SOUND_STAGE_3 = "sound_stageb";					// �{�X��
		constexpr auto SOUND_ENDING = "sound_ending";					// �G���f�B���O
	}
	// �Q�[���N���X
	class Game {
	public:
		// �R���X�g���N�^
		Game();
		// �f�X�g���N�^
		~Game();
		// ������
		void Init();
		// ���͏�Ԃ̍X�V
		void Input();
		// �X�V
		void Process();
		// �`��
		void Draw();
		// �A�i���O�X�e�B�b�N�̉����͂��擾
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		// �A�i���O�X�e�B�b�N�̏c���͂��擾
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		// �A�i���O�X�e�B�b�N�̓��͏����擾�i�������j
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		// �W���C�p�b�h�̑S���͏����擾
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// �g���K�����擾
		inline int GetTrgKey() { return _trgKey; }
		// �t���[���J�E���^���擾
		inline int GetFrameCount() { return _frameCount; }
		// �v���O�������I�������邩�̎擾
		inline bool GetEndFlag() { return _endFlag; }
		// �v���O�����I��������
		inline void ProgramEnd() { ++_endCount; }
		// Game�N���X�̎Q��
		inline Game& GetGame() { return *this; }
#ifdef _DEBUG
		// �f�o�b�O���[�h�t���O�̎擾
		inline bool IsDebugMode() { return _debug; }
#endif
		// ���[�h�T�[�o�̎擾
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		// �I�u�W�F�N�g�T�[�o�̎擾
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		// �M�~�b�N�T�[�o�̎擾
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		// �}�b�v�`�b�v�̎擾
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		// �V�i���I�̎擾
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }
	private:
		std::unique_ptr<ModeServer> _modeServer;	// ���[�h�T�[�o
		std::unique_ptr<ObjectServer> _objServer;	// �I�u�W�F�T�[�o
		std::unique_ptr<GimmickServer> _gServer;	// �M�~�b�N�T�[�o
		std::unique_ptr<MapChips> _mapChips;		// �}�b�v�`�b�v
		std::unique_ptr<Scenario> _scenario;		// �V�i���I
		Vector2 _worldPosition;						// ���[���h���W
		std::tuple<int, int, int> _joyKey;			// �W���C�p�b�h�̓��͏��
		int _endCount;								// �Q�[�����I������܂ł̃J�E���^
		bool _endFlag;								// �v���O�������I�����邩�ۂ�
#ifdef _DEBUG
		bool _debug;								// �f�o�b�O��Ԃ��ۂ�
#endif
		static int _trgKey;							// �g���K�[
		static int _frameCount;						// �t���[�����
		// �t���[���J�E���^�̉��Z����
		void CountUp();
	};
}


