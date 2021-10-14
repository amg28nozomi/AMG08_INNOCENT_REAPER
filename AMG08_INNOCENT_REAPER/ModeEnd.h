/*****************************************************************//**
 * \file   ModeEnd.h
 * \brief  エンディングを管理するモードエンドクラス（モードベースのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {
	// エンディング
	class ModeEnd : public ModeBase {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		/**
		 * コンストラクタ.
		 * 
		 * \param game
		 */
		ModeEnd(Game& game);
		// デストラクタ
		~ModeEnd() = default;
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
	private:
		int _count;								// 待ち時間
		bool _end;								// 処理を終了するか？
		bool _input;							// 入力を受け付けるか？
		std::unique_ptr<MoveImage> _staffRoll;	// スタッフロール
		// ゲームタイトルに遷移するかの判定
		bool IsEnd();
		// 入力は行われたか
		bool IsInput();
	};
}

