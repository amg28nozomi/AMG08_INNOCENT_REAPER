/*****************************************************************//**
 * \file   FadeBlack.h
 * \brief  フェードイン・フェードアウトを行うフェードブラッククラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {
	/** 画像のコンパイル時定数 */
	namespace image {
		// 画像キー
		constexpr auto BLACK = "black";		//!< 黒画像のキー
		// 暗転フラグ
		constexpr auto FADE_OUT = true;		//!< フェードアウト
		constexpr auto FADE_IN = false;		//!< フェードイン
	}
	/** フェードブラッククラス */
	class FadeBlack : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		FadeBlack(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~FadeBlack();
		/**
		 * @brief				初期化処理
		 */
		void Init() override;
		/**
		 * @brief				更新処理
		 */
		void Process() override;
		/**
		 * @brief				描画処理
		 * 
		 */
		void Draw() override;
		/**
		 * @brief						フェードイン・フェードアウト切り替え処理
		 * @param fadeType	フェードイン・フェードアウトのどちらを行うか
		 *									true:フェードアウト　false:フェードイン
		 * @param interval	処理を開始するまでのインターバル
		 */
		void FlagChange(bool fadeType, int interval = 0);
		/**
		 * @brief		終了フラグの取得
		 * @return	終了フラグを返す
		 */
		inline bool IsEnd() { return _end; }
		/**
		 * @brief		加算・減算処理が終了しているかの取得
		 * @return	処理を実行している場合はtrueを返す
		 *					実行中の場合はfalseを返す
		 */
		inline bool PalChange() { return _addEnd; }
	private:
		int _pal;					//!< 輝度
		int _interval;		//!< 処理終了後の待ち時間
		bool _isInterval;	//!< 現在は待ち時間か？
		bool _end;				//!< 処理は終了したか
		bool _type;				//!< 加算・減算処理のどちらを行うか
		bool _addEnd;			//!< 加算・減算処理が終了しているか
		/**
		 * @brief		フェードアウト処理
		 */
		void FadeOut();
		/**
		 * @brief		フェードイン処理
		 */
		void FadeIn();
		/**
		 * @brief		処理が終了しているかの判定
		 * @return	終了している場合はtrueを返す
		 *					終了していない場合はfalseを返す
		 */
		bool FadeEnd();
	};
}

