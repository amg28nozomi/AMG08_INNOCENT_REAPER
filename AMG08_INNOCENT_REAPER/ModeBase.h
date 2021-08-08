#pragma once
#include <unordered_map>

namespace inr {

	namespace mode {
		constexpr auto TITLE = "mode_title";	// �^�C�g��
		constexpr auto SELECT = "mode_select";	// �Z���N�g���
		constexpr auto MAIN = "mode_main";	// �{��
		constexpr auto GAMEOVER = "mode_gameover";	// �Q�[���I�[�o�[
		constexpr auto FIN = "mode_end";	// �I��
	}


	class Game;
	class ObjectServer;
	class ModeServer;

	class ModeBase {
	protected:
		static int _modeFrame;	// �o�߃t���[��
		bool _update;	// �������X�V���邩�ǂ���
		bool _resetFlg; // ���[�h�؂�ւ����ɏ��������邩�ǂ���
		Game& _game;

		using ModeData = std::unordered_map<std::string, int>;
	public:
		ModeBase(Game& game);
		~ModeBase();
		virtual void Init();
		virtual void Process();	
		virtual void Draw();
		inline void Reset() { _resetFlg = true; } // �������t���O�I��
		inline void TimeClear() { _modeFrame = 0; };	// �t���[��������
		inline int GetFrame() { return _modeFrame; }	// �o�߃t���[�����擾
	};
}

//// �^�C�g���A�`���[�g���A���A�{�ҁA�G���f�B���O�A�I��
		//enum class ModeState {
		//	TITLE, TUTORIAL, MAIN_GAME, ENDING, FIN
		//};
//ModeState _mode;	// ���̃��[�h��

//inline ModeState ModeState() { return _mode; }	// State���擾

// 