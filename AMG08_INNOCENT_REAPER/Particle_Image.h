/*****************************************************************//**
 * @file   Particle_Image.h
 * @brief  パーティクル画像クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"

namespace inr {
	/** 画像のコンパイル時定数 */
	namespace image {
		/** パーティクル画像のコンパイル時定数 */
		namespace particle {
			constexpr auto BG_BLACK = "bg_black1";					//!< 黒背景
			constexpr auto BG_OPERATION = "oparation";			//!< 操作説明
			constexpr auto BG_SIZE_W = 1920;								//!< サイズ(幅)
			constexpr auto BG_SIZE_H = 1080;								//!< サイズ(高さ)
			// ポーズUI
			constexpr auto CONTINUE = "continue";						//!< コンティニュー
			constexpr auto CONTROLS = "controls";						//!< コントロール
			constexpr auto QUIT_TO_TITLE = "quit_to_title";	//!< ゲーム終了
			// サイズ
			constexpr auto CONTINUE_W = 272;								//!< コンティニューサイズ(幅)
			constexpr auto CONTROLS_W = 284;								//!< コントロールサイズ(幅)
			constexpr auto QUIT_TO_TITLE_W = 365;						//!< ゲーム終了(幅)
			constexpr auto UI_H_1 = 55;											//!< UI高さ(1)
			constexpr auto UI_H_2 = 64;											//!< UI高さ(2)
			// UI
			constexpr auto CURSOR = "cursor";								//!< カーソル
			constexpr auto CURSOR_W = 500;									//!< カーソルの幅
			constexpr auto CURSOR_H = 150;									//!< カーソルの高さ
			constexpr auto STAGE_UI = "ui_stage";						//!< ステージUI
			constexpr auto STAGE_UI_W = 692;								//!< ステージUIの幅
			constexpr auto STAGE_UI_H = 279;								//!< ステージUIの高さ
			constexpr auto NORMAL = false;									//!< イメージタイプ(通常)
			constexpr auto UI = true;												//!< イメージタイプ(UI)
		}
	}
	/** アニメーションのコンパイル時定数 */
	namespace animation {
		constexpr auto A_SUB = -1;												//!< 減算
		constexpr auto A_NORMAL = 0;											//!< 通常
		constexpr auto A_ADD = 1;													//!< 加算
	}
	/** 二重インクルード防止 */
	class ImageValue;
	/** パーティクル画像 */
	class Particle_Image : public Image {
	protected:
		int _animation;			//!< アニメーションを行うか
		int _pal;						//!< 透明値
		double _maxExt;			//!< 最大拡大率
		double _extRate;		//!< 拡大率
		bool _isDraw;				//!< 描画処理を行っているか
		bool _imageType;		//!< イメージタイプ
		bool _end;					//!< 処理を終了するか
		Vector2 _startPos;	//!< 開始地点
		/**
		 * @brief					輝度の加算処理
		 * @return				輝度が上限に達した場合はtrueを返す
		 *								到達していない場合はfalseを返す
		 */
		bool AddPal();
		/**
		 * @brief					輝度の減算処理
		 * @return				輝度が下限に到達している場合はtrueを返す
		 *								到達していない場合はfalseを返す
		 */
		bool SubPal();
	public:
		/**
		 * @brief					コンストラクタ
		 * @param game		ゲームクラスの参照
		 */
		Particle_Image(Game& game);
		/**
		 * @brief					デストラクタ
		 */
		~Particle_Image() = default;
		/**
		 * @brief					初期化処理
		 */
		void Init() override;
		/**
		 * @brief					更新処理
		 */
		virtual void Process() override;
		/**
		 * @brief					描画処理
		 */
		void Draw() override;
		/**
		 * @brief					画像の設定
		 * @param graph		画像キー
		 * @param pos			生成座標
		 * @param pal			輝度
		 * @param extrate	拡大率
		 */
		void SetParameter(std::string graph, Vector2 pos, int pal = 0, double extrate = 1.0);
		/**
		 * @brief					画像の設定
		 * @param ivalue	画像情報
		 */
		void SetImage(ImageValue ivalue);
		/**
		 * @brief					アニメーション処理
		 * @return				アニメーション処理が終了した場合はtrueを返す
		 *								それ以外の場合はfalseを返す
		 */
		bool Animation();
		/**
		 * @brief					描画開始
		 * @return				処理に成功した場合はtrueを返す
		 *								既に描画が開始している場合はfalseを返す
		 */
		virtual bool DrawStart();
		/**
		 * @brief					描画終了
		 * @return				処理に成功した場合はtrueを返す
		 *								既に描画が終了している場合はfalseを返す
		 */
		bool DrawEnd();
		/**
		 * @brief					終了処理
		 * @return				終了に成功した場合はtrueを返す
		 *								既に終了処理が行われている場合はfalseを返す
		 */
		bool End();
		/**
		 * @brief					イメージタイプの取得
		 * @return				イメージタイプを返す
		 */
		inline bool IsType() { return _imageType; }
		/**
		 * @brief					描画処理フラグの取得
		 * @return				描画処理フラグを返す
		 */
		inline bool IsDraw() { return _isDraw; }
		/**
		 * @brief					通常処理かの判定
		 * @return				通常状態の場合はtrueを返す
		 *								処理がある場合はfalseを返す
		 */
		bool IsNormal();
		virtual void ChangePosition(bool type);
	};
}

