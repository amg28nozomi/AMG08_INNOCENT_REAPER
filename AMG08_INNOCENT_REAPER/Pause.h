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
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Pause(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~Pause();

		void Init();
		void Process();
		void Draw();

		bool IsActive();	// 活動状態に入っているか？
		/**
		 * @brief		活動フラグの取得
		 * @return	活動フラグを返す
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																				//!< ゲームクラスの参照
		std::vector<std::shared_ptr<Particle_Image>> _uis;	//!< 各種UI
		bool _active;																				//!< 活性化しているか
		bool _isEnd;																				//!< ポーズを終了するか？
		bool _input;																				//!< 入力を受け付けているか
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

