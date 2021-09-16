#pragma once
#include <vector>
#include <memory>
#include "Particle_Image.h"
#include "Image.h"

namespace inr {

	class Game;
	// ポーズ機能制御用
	class Pause {
	public:
		Pause(Game& game);
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool IsActive();	// 活動状態に入っているか？
		bool Active() { return _active; }
	private:
		Game& _game;
		std::vector<std::shared_ptr<Particle_Image>> _uis;	// 各種UI

		bool _active;	// 活性化しているか
		bool InputLever();	// レバー
		bool InputButton();	// ボタン
		bool PauseOn();	// キー入力はあったかどうか

		void Sound(std::string soundkey);

		// 必要なもの
		// 背景*
		// UI3種
		// カーソル
		// 操作説明画像*
	};
}

