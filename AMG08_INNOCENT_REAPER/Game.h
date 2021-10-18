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
	constexpr auto GAME_PLAYING = false;			//!< �Q�[�����s��
	constexpr auto GAME_END = true;						//!< �Q�[���I��
	// �W���C�p�b�h�̓��͏����o���p�ԍ�
	constexpr auto LEVER_LR = 0;							//!< �W���C�p�b�h�̏c����
	constexpr auto LEVER_UD = 1;							//!< �W���C�p�b�h�̉�����
	constexpr auto KEY_JOYPAD = 2;						//!< �W���C�p�b�h�̃{�^������
	// ��ʏ��
	constexpr auto WINDOW_W = 1920;						//!< ��ʂ̉��T�C�Y
	constexpr auto WINDOW_H = 1080;						//!< ��ʂ̏c�T�C�Y
	constexpr auto WINDOW_B = 32;							//!< �r�b�g��
	// �n�[�t�T�C�Y
	constexpr auto HALF_WINDOW_W = 1920 / 2;	//!< ��ʔ����̉��T�C�Y
	constexpr auto HALF_WINDOW_H = 1080 / 2;	//!< ��ʔ����̏c�T�C�Y
	// ���@�̃T�C�Y���
	constexpr auto PLAYER_WIDTH = 50;					//!< ���@�̉��T�C�Y
	constexpr auto PLAYER_HEIGHT = 110;				//!< ���@�̏c�T�C�Y
	/**   SE�̃R���p�C�����萔 */
	namespace key {
		// �����p�L�[(SoundServer)
		constexpr auto SOUND_NUM = "num";															//!< ��������
		// ���@SE
		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";							//!< ���@�ړ���SE1
		constexpr auto SOUND_PLAYER_RUN2 = "player_run2";							//!< ���@�ړ���SE2
		constexpr auto SOUND_PLAYER_ROB = "player_rob";								//!< ���@�D��SE
		constexpr auto SOUND_PLAYER_GIVE_TRUE = "player_give_true";		//!< ���@�^����(����)SE
		constexpr auto SOUND_PLAYER_GIVE_FALSE = "player_give_false";	//!< ���@�^����(���s)SE
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";							//!< ���@�W�����vSE
		constexpr auto SOUND_PLAYER_FALL1 = "player_fall_1";					//!< ���@���n1SE
		constexpr auto SOUND_PLAYER_FALL2 = "player_fall_2";					//!< ���@���n2SE
		constexpr auto SOUND_PLAYER_DASH = "player_dash";							//!< ���@�_�b�V��SE
		constexpr auto SOUND_PLAYER_SWITCH = "player_switch";					//!< ���@���̐؂�ւ�SE
		constexpr auto SOUND_PLAYER_HIT = "player_hit";								//!< ���@�_���[�WSE
		constexpr auto SOUND_PLAYER_DEAD = "player_dead";							//!< ���@���SSE
	}
	/** �V�X�e���̃R���p�C�����萔 */
	namespace system {
		constexpr auto SOUDN_GAME_START = "game_start";								//!< �Q�[���J�nSE
		constexpr auto SOUND_GAME_END = "game_end";										//!< �Q�[���I��SE
		constexpr auto SOUDN_CANCEL = "cancel";												//!< �L�����Z��SE
		constexpr auto SOUND_PAUSE = "pause";													//!< �|�[�YSE
		constexpr auto SOUND_SELECT = "select";												//!< �Z���N�g��SE
		constexpr auto MOVE_STAGE1 = "move_stage1";										//!< �X�e�[�W�J��1SE
		constexpr auto MOVE_STAGE2 = "move_stage2";										//!< �X�e�[�W�J��2SE
	}
	/** BGM�̃R���p�C�����萔 */
	namespace bgm {
		// BGM
		constexpr auto SOUND_TITLE = "sound_title";										//!< �^�C�g��(BGM)
		constexpr auto SOUND_STAGE_0 = "sound_stage0";								//!< ����̍Ւd(BGM)
		constexpr auto SOUND_STAGE_1 = "sound_stage1";								//!< �r�p������(BGM)
		constexpr auto SOUND_STAGE_2 = "sound_stage2";								//!< �_��̐X(BGM)
		constexpr auto SOUND_STAGE_3 = "sound_stageb";								//!< �{�X��(BGM)
		constexpr auto SOUND_ENDING = "sound_ending";									//!< �G���f�B���O(BGM)
	}
	/** ��d�C���N���[�h�h�~ */
	class ObjectServer;
	class ModeServer;
	class GimmickServer;
	class Scenario;
	/** �Q�[���N���X */
	class Game {
	public:
		/**
		 * @brief		�R���X�g���N�^
	   */
		Game();
		/**
		 * @brief		�f�X�g���N�^
		 */
		~Game();
		/**
		 * @brief		����������
		 */
		void Init();
		/**
		 * @brief		���͏���
		 */
		void Input();
		/**
		 * @brief		�X�V���� 
		 */
		void Process();
		/**
		 * @brief		�`�揈��
		 */
		void Draw();
		/**
		 * @brief		�A�i���O�X�e�B�b�N�̉����͏����擾
		 * @return	�A�i���O�X�e�B�b�N�̉����͏���Ԃ�
		 */
		inline int GetLeverLR() { return std::get<LEVER_LR>(_joyKey); }
		/**
		 * @brief		�A�i���O�X�e�B�b�N�̏c���͏����擾
		 * @return	�A�i���O�X�e�B�b�N�̏c���͏���Ԃ�
		 */
		inline int GetLeverUD() { return std::get<LEVER_UD>(_joyKey); }
		/**
		 * @brief		�A�i���O�X�e�B�b�N�̃{�^�����͏����擾�i�������j
		 * @return	�A�i���O�X�e�B�b�N�̃{�^�����͏���Ԃ��i�������j
		 */
		inline int GetKey() { return std::get<KEY_JOYPAD>(_joyKey); }
		/**
		 * @brief		�W���C�p�b�h�̑S���͏����擾�i�������j
		 * @return	�W���C�p�b�h�̑S���͏���Ԃ��i�������j
		 */
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		/**
		 * @brief		�g���K�����擾
		 * @return	�g���K����Ԃ�
		 */
		inline int GetTrgKey() { return _trgKey; }
		/**
		 * @brief		�t���[���J�E���^���擾
		 * @return	�t���[���J�E���^��Ԃ�
		 */
		inline int GetFrameCount() { return _frameCount; }
		/**
		 * @brief		�v���O�����I���t���O�̎擾
		 * @return	�v���O�����I���t���O��Ԃ�
		 */
		inline bool GetEndFlag() { return _endFlag; }
		/**
		 * @brief		�v���O�����I���\��
		 */
		inline void ProgramEnd() { ++_endCount; }
		/**
		 * @brief		�Q�[���N���X�̎Q�Ƃ��擾
		 * @return	�Q�Ƃ�Ԃ�
		 */
		inline Game& GetGame() { return *this; }
#ifdef _DEBUG
		/**
		 * @brief		�f�o�b�O�t���O�̎擾(Debug�r���h����)
		 * @return	�f�o�b�O�t���O��Ԃ�
		 */
		inline bool IsDebugMode() { return _debug; }
#endif
		/**
		 * @brief		���[�h�T�[�o�̎擾
		 * @return	���[�h�T�[�o�̎Q�Ƃ�Ԃ�
		 */
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		/**
		 * @brief		�I�u�W�F�N�g�T�[�o�̎擾
		 * @return	�I�u�W�F�N�g�T�[�o�̎Q�Ƃ�Ԃ�
		 */
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		/**
		 * @brief		�M�~�b�N�T�[�o�̎擾
		 * @return	�M�~�b�N�T�[�o�̎Q�Ƃ�Ԃ�
		 */
		inline std::unique_ptr<GimmickServer>& GetGimmickServer() { return _gServer; }
		/**
		 * @brief		�}�b�v�`�b�v�̎擾
		 * @return	�}�b�v�`�b�v�̎Q�Ƃ�Ԃ�
		 */
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }
		/**
		 * @brief		�V�i���I�̎擾
		 * @return	�V�i���I�̎Q�Ƃ�Ԃ�
		 */
		inline std::unique_ptr<Scenario>& GetScenario() { return _scenario; }
	private:
		std::unique_ptr<ModeServer> _modeServer;		//!< ���[�h�T�[�o
		std::unique_ptr<ObjectServer> _objServer;		//!< �I�u�W�F�T�[�o
		std::unique_ptr<GimmickServer> _gServer;		//!< �M�~�b�N�T�[�o
		std::unique_ptr<MapChips> _mapChips;				//!< �}�b�v�`�b�v
		std::unique_ptr<Scenario> _scenario;				//!< �V�i���I
		Vector2 _worldPosition;											//!< ���[���h���W
		std::tuple<int, int, int> _joyKey;					//!< �W���C�p�b�h�̓��͏��
		int _endCount;															//!< �Q�[�����I������܂ł̃J�E���^
		bool _endFlag;															//!< �v���O�������I�����邩�ۂ�
#ifdef _DEBUG
		bool _debug;																//!< �f�o�b�O��Ԃ��̃t���O
#endif
		static int _trgKey;													//!< �g���K���
		static int _frameCount;											//!< �t���[���J�E���^
		/**
		 * @brief	�t���[���J�E���^�̉��Z����
		 */
		void CountUp();
	};
}


