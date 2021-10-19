/*****************************************************************//**
 * @file   Pause.h
 * @brief  ポーズ管理用クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include <memory>
#include "Particle_Image.h"
#include "Image.h"

namespace inr {
	/** 二重インクルード防止 */
	class Game;
	/** ポーズクラス */
	class Pause {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		Pause(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~Pause();
		/**
		 * @brief				初期化処理
		 */
		void Init();
		/**
		 * @brief				更新処理
		 */
		void Process();
		/**
		 * @brief				描画処理
		 */
		void Draw();
		/**
		 * @brief				活動状態の判定
		 * @return			活動状態の場合はtrueを返す
		 *							非活動状態の場合はfalseを返す
		 */
		bool IsActive();
		/**
		 * @brief				活動フラグの取得
		 * @return			活動フラグを返す
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																				//!< ゲームクラスの参照
		std::vector<std::shared_ptr<Particle_Image>> _uis;	//!< 各種UI
		bool _active;																				//!< 活性化しているか
		bool _isEnd;																				//!< ポーズを終了するか？
		bool _input;																				//!< 入力を受け付けているか
		/**
		 * @brief				アナログスティックの入力処理
		 * @return	
		 */
		bool InputLever();
		/**
		 * @brief				ボタンの入力処理
		 * @return			処理に成功した場合はtrueを返す
		 *							失敗or入力がない場合はfalseを返す
		 */
		bool InputButton();
		/**
		 * @brief				ポーズを起動するか
		 * @return			起動に成功した場合はtrueを返す
		 *							失敗した場合はfalseを返す
		 */
		bool PauseOn();
		/**
		 * @brief				効果音の再生
		 */
		void Sound(std::string soundkey);
	};
}

