#pragma once
#include <memory>
#include <vector>

namespace inr {

	class Game;
	class Pause_UI;
	class Particle_Image;
	// ポーズ機能制御用
	class Pause {
	public:
		Pause(Game& game);
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool Active() { return _active; }
	private:
		Game& _game;
		std::unique_ptr<Particle_Image> _bg;	// バックグラウンド
		std::unique_ptr<Particle_Image> _operation;	// 操作説明用画像
		std::vector<std::unique_ptr<Pause_UI>> _uis;	// 各種UI

		bool _active;	// 活性化しているか
		bool IsActive();	// 活動状態に入っているか？
		bool PauseOn();	// キー入力はあったかどうか

		// 必要なもの
		// 背景*
		// UI3種
		// カーソル
		// 操作説明画像*
	};
}

