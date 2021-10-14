/*****************************************************************//**
 * \file   BackGround.h
 * \brief  背景クラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {
	/** ステージのコンパイル時定数 */
	namespace stage {
		/** ステージ番号のコンパイル時定数 */
		namespace number {
			constexpr auto SN_NULL = -1;	//!< 該当なし
			constexpr auto SN_S = 0;			//!< ステージ0
			constexpr auto SN_T = 1;			//!< チュートリアルステージ
			constexpr auto SN_1 = 2;			//!< ステージ1
			constexpr auto SN_2 = 3;			//!< ステージ2
			constexpr auto SN_B = 4;			//!< ボスステージ
		}
	}
	/** 背景 */
	class BackGround : public Image{
	public:
		/**
		 * @brief	コンストラクタ
		 * @param	game	ゲームクラスの参照
		 */
		BackGround(Game& game);		
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
		/**
		 * @brief	ステージに応じた背景に切り替える
		 */
		void ChangeGraph();
		/**
		 * @brief		キーは切り替わったか？
		 * @return	画像番号が異なる場合はtrueを返す
		 * @return	画像番号が等しい場合はfalseを返す
		 */
		bool IsChanege();
		/**
		 * @brief		ステージ番号の判定および取得
		 * @return	ヒットした場合は対応するステージ番号を返す
		 * @return	ヒットしなかった場合は-1を返す
		 */
		int KeyNumber();
		/**
		 * @brief	スクロール処理終了
		 */
		inline void ScrollOff() { _scroll = false; }
		/**
		 * @brief	スクロール処理再開
		 */
		inline void ScrollOn() { _scroll = true; }
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	//!< 描画座標(左辺:1枚目の描画座標, 右辺2枚目の描画座標)
		std::pair<std::vector<double>, std::vector<double>> _scrSpeed;		//!< スクロール移動量(左辺:x, 右辺:y)
		std::pair<int, int> _fix;																					//!< 描画修正値
		int _stageNo;																											//!< 現在のステージ
		bool _scroll;																											//!< スクロールするか
		std::string zKey;																									//!< 前景
		/**
		 * @brief	前描画処理
		 */
		void BackDraw();
		/**
		 * @brief	スクロール処理(大)
		 */
		void BigManage();
		/**
		 * @brief	スクロール処理(小)
		 */
		void NormalManage();
		/**
		 * @brief	Y座標の修正.
		 */
		void ScrollY();
	};
}

