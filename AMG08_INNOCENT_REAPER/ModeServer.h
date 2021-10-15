#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "Game.h"
#include "FadeBlack.h"

namespace inr {

	class Game;
	class ModeMain;

	class ModeServer {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ModeServer(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~ModeServer();

		void Init();
		void Process();
		void Draw();

		void ModeChange(std::string nextMode, int interval = 0);
		void GameEnd();	// 終了処理を呼び出す
		// std::unique_ptr<ModeBase>& GetMode();	// 指定したモードの参照を取得
		std::shared_ptr<ModeMain> GetModeMain();

		inline void FadeOut() { _fadeBlack->FlagChange(image::FADE_OUT, 60); }
		inline bool PalChange() { return _fadeBlack->PalChange(); }
		inline bool IsFadeEnd() { return _fadeBlack->IsEnd(); }
		bool IsFadeChange();
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
