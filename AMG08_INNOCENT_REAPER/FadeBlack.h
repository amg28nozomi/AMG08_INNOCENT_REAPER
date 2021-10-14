/*****************************************************************//**
 * \file   FadeBlack.h
 * \brief  フェードブラッククラス（画像クラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "RedGreenBlue.h"

namespace inr {
	namespace image {
		// 画像キー
		constexpr auto BLACK = "black";
		// 暗転フラグ
		constexpr auto FADE_OUT = true;
		constexpr auto FADE_IN = false;
	}
	// フェードブラッククラス
	class FadeBlack : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		FadeBlack(Game& game);
		// デストラクタ
		~FadeBlack();
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
		// フェードイン・フェードアウト切り替え処理
		// 引数1:フェードイン・フェードアウトのどちらを行うか
		// true:フェードアウト　false:フェードイン
		// 引数2:処理を開始するまでの猶予期間
		void FlagChange(bool fadeType, int interval = 0);
		// 処理は終了しているかの取得
		inline bool IsEnd() { return _end; }
		// 加算・減算処理が終了しているかの取得
		inline bool PalChange() { return _addEnd; }
	private:
		int _pal;			// 輝度
		int _interval;		// 処理終了後の待ち時間
		bool _isInterval;	// 現在は待ち時間か？
		bool _end;			// 処理は終了したか
		bool _type;			// 加算・減算処理のどちらを行うか
		bool _addEnd;		// 加算・減算処理が終了しているか
		// フェードアウト処理
		void FadeOut();
		// フェードイン処理
		void FadeIn();
		// 処理を終了してるかの判定処理
		bool FadeEnd();
	};
}

