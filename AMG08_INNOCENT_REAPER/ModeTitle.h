/*****************************************************************//**
 * @file   ModeTitle.h
 * @brief  タイトルを管理するモードタイトルクラス（モードベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "TitleLogo.h"
#include <memory>

namespace inr {
	/** 二重インクルード防止 */
	class Logo;
	/** モードタイトル */
	class ModeTitle : public ModeBase{
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		ModeTitle(Game& game);
		/**
		 * @brief	デストラクタ
		 */
		~ModeTitle();
		/**
		 * @brief	初期化処理
		 */
		virtual void Init();
		/**
		 * @brief 更新処理
		 */
		virtual void Process();
		/**
		 * @brief 描画処理
		 */
		virtual void Draw();
	private:
		std::unique_ptr<Logo> _titleGh;	//!< タイトルロゴ
	};
}