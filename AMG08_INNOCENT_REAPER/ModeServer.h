#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "Game.h"

namespace inr {

	class Game;

	class ModeServer {
	public:
		ModeServer(Game& game);
		~ModeServer();

		void Init();
		void Process();
		void Draw();

		inline void ModeChange(std::string nextMode) { _modeKey = nextMode; }
	private:
		using ModeMap = std::unordered_map<std::string, std::unique_ptr<ModeBase>>;

		Game& _game;
		ModeMap _modes;		// ���[�h�i�[�p
		std::string _modeKey;	// �����p�L�[���
		std::string _ChangeKey;	// �����L�[�i�[�p

		void ClearModeLists();	// ���[�h�J��
		bool ModeInit();	// ���[�h�̏�����
	};

}
