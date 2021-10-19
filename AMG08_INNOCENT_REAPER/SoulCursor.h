/*****************************************************************//**
 * @file   SoulCursor.h
 * @brief  タイトル時のカーソル
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "RedGreenBlue.h"

namespace inr {
	/** UIのコンパイル時定数 */
	namespace ui {
		constexpr auto CURSOR = "ui_cursor";	//!< カーソルのキー
	}
	/** カーソル */
	class SoulCursor : public ObjectBase {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		SoulCursor(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~SoulCursor() = default;
		/**
		 * @brief					初期化処理
		 */
		void Init() override;
		/**
		 * @brief					更新処理
		 */
		void Process() override;
		/**
		 * @brief					描画処理
		 */
		void Draw() override;
	private:
		Vector2 _moveVector;	//!< 移動量
		RedGreenBlue _rgb;		//!< RGB
		bool _input;					//!< 入力は可能か？	
		bool _setBlend;				//!< 色彩は調整したか？
		/**
		 * @brief					移動処理
		 * @param lever1	アナログスティックの横入力情報
		 * @param lever2	アナログスティックの縦入力情報
		 */
		void Move(int lever1, int lever2);
		/**
		 * @brief					座標の更新
		 */
		void PositionUpdate();
		/**
		 * @brief					RGB値の切り替え
		 */
		void ChangeBlendGraph();
	};
}

