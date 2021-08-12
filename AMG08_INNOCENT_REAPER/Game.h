#pragma once
// #include <memory>
#include <tuple>
#include <memory>
#include "MapChips.h"

// #include <vector>

namespace inr {

	constexpr auto LEVER_LR = 0;
	constexpr auto LEVER_UD = 1;
	constexpr auto KEY_JOYPAD = 2;

	class ObjectServer;
	class ModeServer;

	constexpr auto WINDOW_W = 1920;
	constexpr auto WINDOW_H = 1080;
	constexpr auto WINDOW_B = 32;

	constexpr auto HALF_WINDOW_W = 1920 / 2;
	constexpr auto HALF_WINDOW_H = 1080 / 2;
#ifdef _DEBUG
	constexpr auto DEBUG_WINDOW_W = 1280;
	constexpr auto DEBUG_WINDOW_H = 720;
#endif
	constexpr auto PLAYER_WIDTH = 60;
	constexpr auto PLAYER_HIGHT = 100;

	constexpr auto BACK_GROUND = "backP";
	constexpr auto BACK_HH = "backH";

	namespace key {
		constexpr auto SOUND_NUM = "num";

		constexpr auto SOUND_PLAYER_RUN1 = "player_run1";
		constexpr auto SOUND_PLAYER_ROB = "player_rob";
		constexpr auto SOUND_PLAYER_GIVE = "player_give";
		constexpr auto SOUND_PLAYER_JUMP = "player_jump";
		constexpr auto SOUND_PLAYER_FALL = "player_fall";
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
		// �S���擾
		inline std::tuple<int, int, int> GetInputs() { return _joyKey; }
		// �g���K�����擾
		inline int GetTrgKey() { return _trgKey; }

		inline int GetFrameCount() { return _frameCount; }

		inline Game& GetGame() { return *this; }

		// �e��Q�ƃQ�b�^�[
		inline std::unique_ptr<ModeServer>& GetModeServer() { return _modeServer; }
		inline std::unique_ptr<ObjectServer>& GetObjectServer() { return _objServer; }
		inline std::unique_ptr<MapChips>& GetMapChips() { return _mapChips; }

		// Vector2 RepairPosition(const Vector2 pos, const int width, const int height);

	private:
		// ObjectServer& _objServer;
		std::unique_ptr<ModeServer> _modeServer;	// ���[�h�T�[�o�[
		std::unique_ptr<ObjectServer> _objServer;	// �I�u�W�F�T�[�o�[
		std::unique_ptr<MapChips> _mapChips;	// �}�b�v�`�b�v

		Vector2 _worldPosition;	// world���W


		std::tuple<int, int, int> _joyKey;	// �W���C�p�b�h�̓��͏��

		static int _trgKey;	// �g���K�[
		static int _frameCount;	// �t���[�����
		// int _leverLR;	// �W���C�p�b�h�̃��o�[���(���E)
		// int _leverUD;	// �W���C�p�b�h�̃��o�[���
		// int _inputKey;	// �W���C�p�b�h�̓��͏��
	};
}

