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

		inline void ModeChange(std::string nextMode) { _ChangeKey = nextMode; }
		std::unique_ptr<ModeBase>& GetMode(std::string modeKey);	// 指定したモードの参照を取得
	private:
		using ModeMap = std::unordered_map<std::string, std::unique_ptr<ModeBase>>;

		Game& _game;
		ModeMap _modes;		// モード格納用
		std::string _modeKey;	// 検索用キー情報
		std::string _ChangeKey;	// 検索キー格納用

		void ClearModeLists();	// モード開放
		bool ModeInit();	// モードの初期化
	};

}
