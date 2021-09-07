#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "Game.h"

namespace inr {

	class Game;
	class ModeMain;
	class FadeBlack;

	class ModeServer {
	public:
		ModeServer(Game& game);
		~ModeServer();

		void Init();
		void Process();
		void Draw();

		void ModeChange(std::string nextMode);
		void GameEnd();	// 終了処理を呼び出す
		// std::unique_ptr<ModeBase>& GetMode();	// 指定したモードの参照を取得
		std::shared_ptr<ModeMain> GetModeMain();
	private:
		using ModeMap = std::unordered_map<std::string, std::shared_ptr<ModeBase>>;

		Game& _game;
		ModeMap _modes;		// モード格納用
		std::unique_ptr<FadeBlack> _fadeBlack;
		std::string _modeKey;	// 検索用キー情報
		std::string _ChangeKey;	// 検索キー格納用

		void ClearModeLists();	// モード開放
		bool ModeInit();	// モードの初期化

		void IsModeChange();	// モード切り替え
	};

}
