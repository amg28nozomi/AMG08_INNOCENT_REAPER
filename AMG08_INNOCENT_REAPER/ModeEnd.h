/*****************************************************************//**
 * @file   ModeEnd.h
 * @brief  エンディングを管理するモードエンドクラス（モードベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {
	/** エンディング */
	class ModeEnd : public ModeBase {
	public:
		/**
		 * @brief	コンストラクタ
		 * @param	game	ゲームクラスの参照
		 */
		ModeEnd(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~ModeEnd() = default;
		/**
		 * @brief	初期化処理
		 */
		void Init() override;
		/**
		 * @brief	更新処理
		 */
		void Process() override;
		/**
		 * @brief	描画処理
		 */
		void Draw() override;
	private:
		int _count;															//!< 待ち時間
		bool _end;															//!< 処理を終了するか？
		bool _input;														//!< 入力を受け付けるか？
		std::unique_ptr<MoveImage> _staffRoll;	//!< スタッフロール
		/**
		 * @brief	ゲームタイトルに遷移するかの判定
		 * @return	タイトルに遷移する場合はtrueを返す
		 * @return	遷移しない場合はfalseを返す
		 */
		bool IsEnd();
		/**
		 * @brief	Bボタンが押されたかの判定を行う
		 * @brief	押された場合はタイトル画面に遷移する
		 * @return	タイトル画面に遷移する場合はtrueを返す
		 * @return	遷移しない場合はfalseを返す
		 */
		bool IsInput();
	};
}

